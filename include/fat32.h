#ifndef FAT32_H
#define FAT32_H

#include "boot_sector.h"
#include "device.h"

class Fat32 {
public:
  static char *format_new_fat32(int bytes_per_sector, int table_size_32);

  class FileAddressIndexIterator {
  public:
    FileAddressIndexIterator(Fat32 *fat32, unsigned int current_address_index);

    unsigned int next();
    bool has_next();

  private:
    Fat32 *fat32;
    unsigned int current_address_index;
  };

  static const unsigned char ADDRESS_BYTE_SIZE = 4;

  unsigned int fat_byte_size;
  unsigned short bytes_per_sector;
  unsigned char sectors_per_cluster;
  char *bytes;

  Fat32(Device *device, BootSector *boot_sector);
  void reload(Device *device, BootSector *boot_sector);

  unsigned int get_address(unsigned int index);
  unsigned int get_file_cluster_size(unsigned int index);

  unsigned int get_next_free_cluster();

  FileAddressIndexIterator *get_file_address_index_iterator(unsigned int index);

  void display();
};

#endif