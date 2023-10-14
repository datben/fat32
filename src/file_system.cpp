#include "../include/file_system.h"
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

void FileSystem::reload_fat32() {
  delete fat32;
  fat32 = new Fat32(device, boot_sector);
}
char *FileSystem::get_data(int address_index) {
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