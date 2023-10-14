#include "../include/boot_sector.h"
#include "../include/device.h"
#include "../include/utils.h"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include "../include/utils.h"
using namespace std;

BootSector::BootSector(char *buffer) { BootSector::load(buffer); };

BootSector::BootSector(Device *device) {

  char *buffer = new char[BootSector::BYTE_SIZE];
  device->read(buffer, BootSector::BYTE_SIZE, 0);
  BootSector::load(buffer);
  delete[] buffer;
};

void BootSector::load(char *buffer) {

  char *&buffer_viewer = buffer;
  jump_boot = read_bytes(buffer_viewer, 3);
  oem_name = read_bytes(buffer_viewer, 8);
  bytes_per_sector = BigEndian::read_short(buffer_viewer);
  sectors_per_cluster = BigEndian::read_char(buffer_viewer);
  reserved_sector_count = BigEndian::read_short(buffer_viewer);
  table_count = BigEndian::read_char(buffer_viewer);
  root_entry_count = BigEndian::read_short(buffer_viewer);
  total_sectors_16 = BigEndian::read_short(buffer_viewer);
  media_type = BigEndian::read_char(buffer_viewer);
  table_size_16 = BigEndian::read_short(buffer_viewer);
  sectors_per_track = BigEndian::read_short(buffer_viewer);
  head_side_count = BigEndian::read_short(buffer_viewer);
  hidden_sector_count = BigEndian::read_int(buffer_viewer);
  total_sectors_32 = BigEndian::read_int(buffer_viewer);
  table_size_32 = BigEndian::read_int(buffer_viewer);
  extended_flags = BigEndian::read_short(buffer_viewer);
  fat_version = BigEndian::read_short(buffer_viewer);
  root_cluster = BigEndian::read_int(buffer_viewer);
  fat_info = BigEndian::read_short(buffer_viewer);
  backup_boot_sector = BigEndian::read_short(buffer_viewer);
  reserved_0 = read_bytes(buffer_viewer, 12);
  drive_number = BigEndian::read_char(buffer_viewer);
  reserved_1 = BigEndian::read_char(buffer_viewer);
  boot_signature = BigEndian::read_char(buffer_viewer);
  volume_id = BigEndian::read_int(buffer_viewer);
  volume_label = read_bytes(buffer_viewer, 11);

  fs_type = read_bytes(buffer_viewer, 8);
}

void BootSector::display() {
  cout << "Jump Boot: ";
  print_hex(jump_boot, 3, true);
  cout << "OEM Name: ";
  print_char(oem_name, 8, true);
  cout << "Bytes Per Sector: " << bytes_per_sector << endl;
  cout << "Sectors Per Cluster: " << (int)sectors_per_cluster << endl;
  cout << "Reserved Sector Count: " << reserved_sector_count << endl;
  cout << "Table Count: " << (int)table_count << endl;
  cout << "Root Entry Count: " << root_entry_count << endl;
  cout << "Total Sectors 16: " << total_sectors_16 << endl;
  cout << "Media Type: " << (int)media_type << endl;
  cout << "Table Size 16: " << table_size_16 << endl;
  cout << "Sectors Per Track: " << sectors_per_track << endl;
  cout << "Head Side Count: " << head_side_count << endl;
  cout << "Hidden Sector Count: " << hidden_sector_count << endl;
  cout << "Total Sectors 32: " << total_sectors_32 << endl;
  cout << "Table Size 32: " << table_size_32 << endl;
  cout << "Extended Flags: " << extended_flags << endl;
  cout << "FAT Version: " << fat_version << endl;
  cout << "Root Cluster: " << root_cluster << endl;
  cout << "FAT Info: " << fat_info << endl;
  cout << "Backup Boot Sector: " << backup_boot_sector << endl;
  cout << "Reserved 0: " << reserved_0 << endl;
  cout << "Drive Number: " << (int)drive_number << endl;
  cout << "Reserved 1: " << (int)reserved_1 << endl;
  cout << "Boot Signature: " << (int)boot_signature << endl;
  cout << "Volume ID: " << volume_id << endl;
  cout << "Volume Label: ";
  print_char(volume_label, 11, true);
  cout << "FS Type: ";
  print_char(fs_type, 8, true);
}

unsigned int BootSector::data_offset() {
  return reserved_sector_count * bytes_per_sector + table_count * table_size_32 * bytes_per_sector;
}

unsigned int BootSector::bytes_per_cluster() { return bytes_per_sector * sectors_per_cluster; }