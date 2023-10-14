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
  char *get_data(int address_index);
  void mount(char *path);
  void unmount();

private:
  void reload_fat32();

  char *read_sector(int sector_index);
  char *read_cluster(int cluster_index);
};

#endif