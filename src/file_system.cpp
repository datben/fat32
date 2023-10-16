#include "../include/file_system.h"
#include "../include/device_formater.h"
#include "../include/file.h"
#include "../include/utils.h"
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

FileSystem::FileSystem() { is_mounted = false; }

void FileSystem::mount(char *path) {
  device = new Device(path);
  boot_sector = new BootSector(device);
  fat32 = new Fat32(device, boot_sector);
  current_directory_index = boot_sector->root_cluster;
  is_mounted = true;
}

void FileSystem::unmount() {
  update_fat32();
  delete fat32;
  delete boot_sector;
  delete device;
  is_mounted = false;
}

void FileSystem::format(int bytes_per_sector, int sectors_per_cluster, int reserved_sector_count, int table_size_32) {
  delete fat32;
  delete boot_sector;

  DeviceFormater::format(this, device, bytes_per_sector, sectors_per_cluster, reserved_sector_count, table_size_32);

  current_directory_index = boot_sector->root_cluster;
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
  int bpc = boot_sector->bytes_per_cluster();
  char *data = new char[file_cluster_size * bpc];
  Fat32::FileAddressIndexIterator *iter = fat32->get_file_address_index_iterator(address_index);
  int i = 0;
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

int FileSystem::cd(string name) {
  int file_address = find_file_address(name);
  if (file_address < boot_sector->root_cluster) {
    return -1;
  }
  current_directory_index = file_address;
  return 0;
}

string FileSystem::ls() {
  char *dir_data = get_file_data(current_directory_index);
  int data_size = fat32->get_file_cluster_size(current_directory_index) * boot_sector->bytes_per_cluster();
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

int FileSystem::find_file_address(string name) {
  char *dir_data = get_file_data(current_directory_index);
  int data_size = fat32->get_file_cluster_size(current_directory_index) * boot_sector->bytes_per_cluster();
  DirectoryIterator *iter = new DirectoryIterator(dir_data, data_size);
  int result = -1;
  string resolved_name = resolve_name(name, 11);
  while (iter->has_next()) {
    File file = iter->next();
    if (resolved_name.compare(resolve_name(file.name, 11)) == 0) {
      result = file.get_first_cluster();
      break;
    }
  }
  delete iter;
  delete[] dir_data;
  return result;
}

void FileSystem::clear_cluster(int cluster_index) {
  char *cluster = new char[boot_sector->bytes_per_cluster()]();
  write_cluster(cluster, cluster_index);
  delete[] cluster;
}

void FileSystem::touch(string name) {

  int file_cluster_size = 1;
  int file_size = file_cluster_size * boot_sector->bytes_per_cluster();
  int file_address_index = fat32->get_next_free_cluster();

  File file = File();
  file.name = new char[11];
  strcpy(file.name, resolve_name(name, 11).c_str());
  file.attributes = ARCHIVE;
  file.first_cluster_high = file_address_index >> 16;
  file.first_cluster_low = file_address_index & 0xFFFF;
  file.file_size = file_size;

  char *file_data = file.serialize();

  append_data(current_directory_index, file_data, File::BYTE_SIZE, true);

  delete[] file_data;

  fat32->set_address(file_address_index, 0x0fffffff);

  clear_cluster(file_address_index);
}

void FileSystem::echo(string name, string data) {

  int file_address = find_file_address(name);
  if (file_address == -1) {
    touch(name);
    file_address = find_file_address(name);
  }
  append_data(file_address, data.c_str(), data.size(), false);
}

string FileSystem::cat(string name) {
  int file_address = find_file_address(name);
  if (file_address == -1) {
    return "File not found";
  }
  char *file_data = get_file_data(file_address);
  string result = string(file_data);
  delete[] file_data;
  return result;
}

void FileSystem::append_data(int address_index, const char *data, int size, bool is_dir) {
  unsigned int last_cluster = fat32->last_cluster(address_index);
  char *last_cluster_data = read_cluster(last_cluster);
  int cluster_size = boot_sector->bytes_per_cluster();

  int i = 0;
  int j = 1;
  if (is_dir) {
    j = File::BYTE_SIZE;
  }
  while (i < cluster_size && last_cluster_data[i] != 0) {
    i += j;
  }
  int last_cluster_remaining_size = cluster_size - i;

  if (last_cluster_remaining_size < size) {
    int next_cluster = fat32->get_next_free_cluster();
    fat32->set_address(last_cluster, next_cluster);
    fat32->set_address(next_cluster, 0x0fffffff);

    memccpy(last_cluster_data + i, data, 0, last_cluster_remaining_size);
    write_cluster(last_cluster_data, last_cluster);

    int remaining_data_size = size - last_cluster_remaining_size;

    while (remaining_data_size > 0) {
      int next_cluster = fat32->get_next_free_cluster();
      fat32->set_address(last_cluster, next_cluster);
      fat32->set_address(next_cluster, 0x0fffffff);

      char *next_cluster_data = new char[cluster_size]();
      memcpy(next_cluster_data, data + last_cluster_remaining_size, remaining_data_size % cluster_size);
      write_cluster(next_cluster_data, next_cluster);

      last_cluster_remaining_size += cluster_size;
      remaining_data_size -= cluster_size;

      delete[] next_cluster_data;
    }
  } else {
    memcpy(last_cluster_data + i, data, size);
    write_cluster(last_cluster_data, last_cluster);
  }
  delete[] last_cluster_data;
}

void FileSystem::mkdir(string name) {
  int file_cluster_size = 1;
  int file_size = file_cluster_size * boot_sector->bytes_per_cluster();
  int dir_address_index = fat32->get_next_free_cluster();

  File dir = File();
  dir.name = (char *)".          ";
  dir.attributes = DIRECTORY;
  dir.first_cluster_high = dir_address_index >> 16;
  dir.first_cluster_low = dir_address_index & 0xFFFF;
  dir.file_size = file_size;

  File parent_dir = File();
  parent_dir.name = (char *)"..         ";
  parent_dir.attributes = DIRECTORY;
  parent_dir.first_cluster_high = current_directory_index >> 16;
  parent_dir.first_cluster_low = current_directory_index & 0xFFFF;

  char *dir_data = dir.serialize();
  char *parent_dir_data = parent_dir.serialize();

  char *data = new char[boot_sector->bytes_per_cluster()]();
  memcpy(data, dir_data, File::BYTE_SIZE);
  memcpy(data + File::BYTE_SIZE, parent_dir_data, File::BYTE_SIZE);

  delete[] dir_data;
  delete[] parent_dir_data;

  write_cluster(data, dir_address_index);

  delete[] data;

  fat32->set_address(dir_address_index, 0x0fffffff);

  dir = File();
  dir.name = new char[11]();
  strcpy(dir.name, resolve_name(name, 11).c_str());
  dir.attributes = DIRECTORY;
  dir.first_cluster_high = dir_address_index >> 16;
  dir.first_cluster_low = dir_address_index & 0xFFFF;
  dir.file_size = file_size;
  dir_data = dir.serialize();
  append_data(current_directory_index, dir_data, File::BYTE_SIZE, true);
  delete[] dir_data;
}
