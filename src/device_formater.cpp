#include "../include/device_formater.h"
#include "../include/boot_sector.h"
#include "../include/fat32.h"
#include "../include/file.h"
#include "../include/file_system.h"

#include <cstring>

void DeviceFormater::format(FileSystem *sys, Device *device, int bytes_per_sector, int sectors_per_cluster,
                            int reserved_sector_count, int table_size_32) {
  BootSector *boot_sector = new BootSector();
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

  File dir = File();
  dir.name = (char *)".          ";
  dir.attributes = DIRECTORY;
  dir.reserved = 0x0;
  dir.creation_time = 0x0;
  dir.creation_date = 0x0;
  dir.last_access_date = 0x0;
  dir.first_cluster_high = 2 >> 16;
  dir.last_write_time = 0x0;
  dir.last_write_date = 0x0;
  dir.first_cluster_low = 2 & 0xFFFF;
  dir.file_size = 0;

  File parent_dir = File();
  parent_dir.name = (char *)"..         ";
  parent_dir.attributes = DIRECTORY;
  parent_dir.reserved = 0x0;
  parent_dir.creation_time = 0x0;
  parent_dir.creation_date = 0x0;
  parent_dir.last_access_date = 0x0;
  parent_dir.first_cluster_high = 0 >> 16;
  parent_dir.last_write_time = 0x0;
  parent_dir.last_write_date = 0x0;
  parent_dir.first_cluster_low = 0 & 0xFFFF;
  parent_dir.file_size = 0x0;

  char *dir_data = dir.serialize();
  char *parent_dir_data = parent_dir.serialize();

  char *data = new char[boot_sector->bytes_per_cluster()]();
  memcpy(data, dir_data, File::BYTE_SIZE);
  memcpy(data + File::BYTE_SIZE, parent_dir_data, File::BYTE_SIZE);

  sys->write_cluster(data, 2);

  delete dir_data;
  delete parent_dir_data;
}