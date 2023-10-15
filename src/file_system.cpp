#include "../include/file_system.h"
#include "../include/device_formater.h"
#include "../include/file.h"
#include "../include/utils.h"
#include <cstring>
#include <iostream>

using namespace std;

FileSystem::FileSystem() { is_mounted = false; }

void FileSystem::mount(char *path) {
  device = new Device(path);
  boot_sector = new BootSector(device);
  fat32 = new Fat32(device, boot_sector);
  current_file_index = boot_sector->root_cluster;
  is_mounted = true;
}

void FileSystem::unmount() {
  delete fat32;
  delete boot_sector;
  delete device;
  is_mounted = false;
}

void FileSystem::format(int bytes_per_sector, int sectors_per_cluster, int reserved_sector_count, int table_size_32) {
  delete fat32;
  delete boot_sector;

  DeviceFormater::format(this, device, bytes_per_sector, sectors_per_cluster, reserved_sector_count, table_size_32);

  current_file_index = boot_sector->root_cluster;
  boot_sector = new BootSector(device);
  fat32 = new Fat32(device, boot_sector);

  is_mounted = true;
}

void FileSystem::reload_fat32() {
  delete fat32;
  fat32 = new Fat32(device, boot_sector);
}

void FileSystem::update_fat32() {
  device->write(fat32->bytes, boot_sector->bytes_per_sector * boot_sector->table_size_32,
                boot_sector->reserved_sector_count * boot_sector->bytes_per_sector);
}

char *FileSystem::get_file_data(int address_index) {
  int file_cluster_size = fat32->get_file_cluster_size(address_index);
  char *data = new char[file_cluster_size * boot_sector->bytes_per_cluster()];
  Fat32::FileAddressIndexIterator *iter = fat32->get_file_address_index_iterator(address_index);
  int i = 0;
  int bpc = boot_sector->bytes_per_cluster();
  while (iter->has_next()) {
    int cluster_index = iter->next();
    char *cluster = read_cluster(cluster_index);
    memcpy(data + i * bpc, cluster, bpc);
    delete cluster;
    i++;
  }
  delete iter;
  return data;
}

string FileSystem::ls() {
  char *dir_data = get_file_data(current_file_index);
  int data_size = fat32->get_file_cluster_size(current_file_index) * boot_sector->bytes_per_cluster();
  DirectoryIterator *iter = new DirectoryIterator(dir_data, data_size);
  string result = "";
  while (iter->has_next()) {
    File file = iter->next();
    result.append(file.name, 11);
    result += "\n";
  }
  delete iter;
  delete[] dir_data;
  return result;
}

char *FileSystem::read_sector(int sector_index) {
  char *bytes = new char[boot_sector->bytes_per_sector];
  device->read(bytes, boot_sector->bytes_per_sector, sector_index * boot_sector->bytes_per_sector);
  return bytes;
}

char *FileSystem::read_cluster(int cluster_index) {
  char *bytes = new char[boot_sector->bytes_per_cluster()];
  device->read(bytes, boot_sector->bytes_per_cluster(),
               boot_sector->data_offset() + cluster_index * boot_sector->bytes_per_cluster());
  return bytes;
}

void FileSystem::write_sector(char *buffer, int sector_index) {
  device->write(buffer, boot_sector->bytes_per_sector, sector_index * boot_sector->bytes_per_sector);
}

void FileSystem::write_cluster(char *buffer, int cluster_index) {
  device->write(buffer, boot_sector->bytes_per_cluster(),
                boot_sector->data_offset() + cluster_index * boot_sector->bytes_per_cluster());
}

int FileSystem::find_file(char *name) {
  char *dir_data = get_file_data(current_file_index);
  int data_size = fat32->get_file_cluster_size(current_file_index) * boot_sector->bytes_per_cluster();
  DirectoryIterator *iter = new DirectoryIterator(dir_data, data_size);
  int result = -1;
  while (iter->has_next()) {
    File file = iter->next();
    if (strcmp(file.name, name) == 0) {
      result = file.get_first_cluster();
      break;
    }
  }
  delete iter;
  delete[] dir_data;
  return result;
}

void FileSystem::create_directory(char *name) {
  int file_cluster_size = 1;
  int file_size = file_cluster_size * boot_sector->bytes_per_cluster();
  int dir_address_index = fat32->get_next_free_cluster();

  File dir = File();
  dir.name = (char *)".          ";
  dir.attributes = DIRECTORY;
  dir.reserved = 0x0;
  dir.creation_time = 0x0;
  dir.creation_date = 0x0;
  dir.last_access_date = 0x0;
  dir.first_cluster_high = dir_address_index >> 16;
  dir.last_write_time = 0x0;
  dir.last_write_date = 0x0;
  dir.first_cluster_low = dir_address_index & 0xFFFF;
  dir.file_size = file_size;

  File parent_dir = File();
  parent_dir.name = (char *)"..         ";
  parent_dir.attributes = DIRECTORY;
  parent_dir.reserved = 0x0;
  parent_dir.creation_time = 0x0;
  parent_dir.creation_date = 0x0;
  parent_dir.last_access_date = 0x0;
  parent_dir.first_cluster_high = current_file_index >> 16;
  parent_dir.last_write_time = 0x0;
  parent_dir.last_write_date = 0x0;
  parent_dir.first_cluster_low = current_file_index & 0xFFFF;
  parent_dir.file_size = 0x0;

  char *dir_data = dir.serialize();
  char *parent_dir_data = parent_dir.serialize();

  char *data = new char[boot_sector->bytes_per_cluster()]();
  memcpy(data, dir_data, File::BYTE_SIZE);
  memcpy(data + File::BYTE_SIZE, parent_dir_data, File::BYTE_SIZE);

  write_cluster(data, dir_address_index);

  fat32->set_address(dir_address_index, 0x0fffffff);

  dir.name = name;
  dir_data = dir.serialize();

  char *current_dir_data = get_file_data(current_file_index);
  int data_size = fat32->get_file_cluster_size(current_file_index) * boot_sector->bytes_per_cluster();
  DirectoryIterator *iter = new DirectoryIterator(current_dir_data, data_size);
  int i = 0;
  while (iter->has_next()) {
    iter->next();
    i++;
  }
  // todo add file to current dir

  delete iter;

  delete[] current_dir_data;
  delete[] dir_data;
  delete[] parent_dir_data;
}