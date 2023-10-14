#ifndef FAT32_H
#define FAT32_H

#include "device.h"

class Fat32 {
public:
  unsigned int fat_byte_size;
  char *bytes;

  Fat32(Device *device, BootSector *boot_sector);

  void display();
};

#endif