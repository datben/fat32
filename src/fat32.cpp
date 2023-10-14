#include "../include/fat32.h"
#include "../include/boot_sector.h"
#include "../include/device.h"

Fat32::Fat32(Device *device, BootSector *boot_sector) {
  fat_byte_size = boot_sector->bytes_per_sector * boot_sector->table_size_32;
  bytes = new char[fat_byte_size];
  device->read(bytes, fat_byte_size,
               boot_sector->reserved_sector_count * boot_sector->bytes_per_sector);
}
