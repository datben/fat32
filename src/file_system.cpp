#include "../include/file_system.h"
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

  boot_sector = new BootSector();
  boot_sector->bytes_per_sector = bytes_per_sector;
  boot_sector->sectors_per_cluster = sectors_per_cluster;
  boot_sector->reserved_sector_count = reserved_sector_count;
  boot_sector->table_size_32 = table_size_32;
  boot_sector->root_entry_count = 0;
  boot_sector->fat_version = 0x0;
  boot_sector->fs_type = (char *)"FAT32   ";
  boot_sector->volume_label = (char *)"NO NAME    ";
  boot_sector->volume_id = 0x0;
  boot_sector->table_count = 0x1;
  boot_sector->media_type = 0xF8;
  boot_sector->fat_info = 0x0;
  boot_sector->backup_boot_sector = 0x0;
  boot_sector->drive_number = 0x80;
  boot_sector->boot_signature = 0x29;
  boot_sector->reserved_1 = 0x0;
  boot_sector->reserved_0 = new char[12];
  for (int i = 0; i < 12; i++) {
    boot_sector->reserved_0[i] = 0x0;
  }
  boot_sector->extended_flags = 0x0;
  boot_sector->root_cluster = 0x2;
  boot_sector->total_sectors_32 = 0x0;
  boot_sector->hidden_sector_count = 0x0;
  boot_sector->head_side_count = 0x0;
  boot_sector->sectors_per_track = 0x0;
  boot_sector->table_size_16 = 0x0;
  boot_sector->total_sectors_16 = 0x0;
  boot_sector->oem_name = (char *)"MSWIN4.1";
  boot_sector->jump_boot = new char[3];
  boot_sector->jump_boot[0] = 0xEB;
  boot_sector->jump_boot[1] = 0x58;
  boot_sector->jump_boot[2] = 0x90;

  char *bs_data = boot_sector->serialize();
  device->write(bs_data, BootSector::BYTE_SIZE, 0);
  delete bs_data;

  char *fat32_data = Fat32::format_new_fat32(bytes_per_sector, table_size_32);
  device->write(fat32_data, bytes_per_sector * table_size_32, reserved_sector_count * bytes_per_sector);
  delete fat32_data;

  fat32 = new Fat32(device, boot_sector);
  is_mounted = true;
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