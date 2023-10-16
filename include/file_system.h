#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "boot_sector.h"
#include "device.h"
#include "fat32.h"

#include <string>

using namespace std;

class FileSystem {
public:
  Device *device;
  BootSector *boot_sector;
  Fat32 *fat32;

  int current_directory_index;

  bool is_mounted;

  FileSystem();

  void mount(char *path);
  void unmount();

  void mkdir(string name);
  void touch(string name);
  void echo(string name, string data);
  string cat(string name);
  string ls();

  int find_file_address(string name);

  char *get_file_data(int address_index);

  void format(int bytes_per_sector, int sectors_per_cluster, int reserved_sector_count, int table_size_32);

  void reload_fat32();
  void update_fat32();

  void append_data(int address_index, const char *data, int size, bool is_dir);

  char *read_sector(int sector_index);
  char *read_cluster(int cluster_index);

  void clear_cluster(int cluster_index);

  void write_sector(char *buffer, int sector_index);
  void write_cluster(char *buffer, int cluster_index);
};

#endif