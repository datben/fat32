#include "../include/fat32.h"
#include "../include/boot_sector.h"
#include "../include/utils.h"
#include <cstring>
#include <iostream>

#include <stdio.h>

using namespace std;

Fat32::Fat32(Device *device, BootSector *boot_sector) {
  fat_byte_size = boot_sector->bytes_per_sector * boot_sector->table_size_32;
  bytes = new char[fat_byte_size];
  device->read(bytes, fat_byte_size, boot_sector->reserved_sector_count * boot_sector->bytes_per_sector);
}

void Fat32::reload(Device *device, BootSector *boot_sector) {
  fat_byte_size = boot_sector->bytes_per_sector * boot_sector->table_size_32;
  bytes = new char[fat_byte_size];
  device->read(bytes, fat_byte_size, boot_sector->reserved_sector_count * boot_sector->bytes_per_sector);
}

void Fat32::display() {
  cout << "FAT32" << endl;
  print_hex(bytes, fat_byte_size, true);
}

unsigned int Fat32::get_address(unsigned int index) {
  unsigned int address = 0;
  memcpy(&address, bytes + index * ADDRESS_BYTE_SIZE, ADDRESS_BYTE_SIZE);
  return address;
}

unsigned int Fat32::get_file_cluster_size(unsigned int index) {
  int cluster_size = 0;
  FileAddressIndexIterator *iterator = get_file_address_index_iterator(index);
  while (iterator->has_next()) {
    iterator->next();
    cluster_size++;
  }
  delete iterator;
  return cluster_size;
}

Fat32::FileAddressIndexIterator *Fat32::get_file_address_index_iterator(unsigned int index) {
  return new FileAddressIndexIterator(this, index);
}

Fat32::FileAddressIndexIterator::FileAddressIndexIterator(Fat32 *fat32, unsigned int current_address_index) {
  this->fat32 = fat32;
  this->current_address_index = current_address_index;
}

unsigned int Fat32::FileAddressIndexIterator::next() {
  unsigned int index = current_address_index;
  current_address_index = fat32->get_address(current_address_index);
  return index;
}

bool Fat32::FileAddressIndexIterator::has_next() { return current_address_index != 0xffffff0f; }
