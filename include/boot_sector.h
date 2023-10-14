#ifndef BOOT_SECTOR_H
#define BOOT_SECTOR_H

#include "device.h"

class BootSector {
public:
  static const unsigned char BYTE_SIZE = 90;

  char *jump_boot; // 3
  char *oem_name;  // 8
  short bytes_per_sector;
  char sectors_per_cluster;
  short reserved_sector_count;
  char table_count;
  short root_entry_count;
  short total_sectors_16;
  char media_type;
  short table_size_16;
  short sectors_per_track;
  short head_side_count;
  int hidden_sector_count;
  int total_sectors_32;
  int table_size_32;
  short extended_flags;
  short fat_version;
  int root_cluster;
  short fat_info;
  short backup_boot_sector;
  char *reserved_0; // 12
  char drive_number;
  char reserved_1;
  char boot_signature;
  int volume_id;
  char *volume_label; // 11
  char *fs_type;      // 8

  BootSector();
  BootSector(char *buffer);
  BootSector(Device *device);

  void display();
  void load(char *buffer);
  unsigned int data_offset();
  unsigned int bytes_per_cluster();
};

#endif