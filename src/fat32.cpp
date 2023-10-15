#include "../include/fat32.h"
#include "../include/boot_sector.h"
#include "../include/utils.h"
#include <cstring>
#include <iostream>
#include <stdio.h>

using namespace std;

char *Fat32::format_new_fat32(int bytes_per_sector, int table_size_32) {
  char *bytes = new char[bytes_per_sector * table_size_32];
  for (int i = 0; i < bytes_per_sector * table_size_32; i++) {
    bytes[i] = 0x0;
  }
  char *ref = bytes;
  char *&bytes_viewer = ref;
  LittleEndian::write_int(bytes_viewer, 0xf0fffff0);
  LittleEndian::write_int(bytes_viewer, 0x0fffffff);
  LittleEndian::write_int(bytes_viewer, 0x0fffffff);
  return bytes;
}

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
  cout << "FAT32 : " << endl;
  print_hex(bytes, fat_byte_size, true);
}

void Fat32::set_address(unsigned int index, unsigned int value) {
  char *bytes_tab = bytes;
  char *&bytes_viewer = bytes_tab;
  bytes_viewer += index * ADDRESS_BYTE_SIZE;
  LittleEndian::write_int(bytes_viewer, value);
}

unsigned int Fat32::get_address(unsigned int index) {
  char *bytes_tab = bytes;
  char *&bytes_viewer = bytes_tab;
  bytes_viewer += index * ADDRESS_BYTE_SIZE;
  unsigned int address = (unsigned int)LittleEndian::read_int(bytes_viewer);
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

unsigned int Fat32::get_next_free_cluster() {
  unsigned int index = 2;
  while (get_address(index) != 0x0) {
    index++;
  }
  return index;
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

bool Fat32::FileAddressIndexIterator::has_next() { return current_address_index != 0xffffffff; }
