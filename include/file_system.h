#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "boot_sector.h"
#include "device.h"
#include "fat32.h"

class FileSystem {
public:
  Device *device;
  BootSector *boot_sector;
  Fat32 *fat32;

  int current_file_index;

  bool is_mounted;

  FileSystem();

  void mount(char *path);
  void unmount();

  void format(int bytes_per_sector, int sectors_per_cluster, int reserved_sector_count, int table_size_32);

  char *get_data(int address_index);

  void create_directory(char *name);

private:
  void reload_fat32();
  void update_fat32();

  char *read_sector(int sector_index);
  char *read_cluster(int cluster_index);

  void write_sector(char *buffer, int sector_index);
  void write_cluster(char *buffer, int cluster_index);
};

#endif