#include "../include/boot_sector.h"
#include "../include/utils.h"
#include "../include/device.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <cstring>
#include <iostream>
using namespace std;

BootSector::BootSector(char *buffer)
{
    BootSector::load(buffer);
};

BootSector::BootSector(Device *device)
{

    char *buffer = new char[BootSector::BYTE_SIZE];
    device->read(buffer, BootSector::BYTE_SIZE, 0);
    BootSector::load(buffer);
    delete buffer;
};

void BootSector::load(char *buffer)
{
    memcpy(jump_boot, buffer, 3);
    memcpy(oem_name, buffer + 3, 8);
    memcpy(&bytes_per_sector, buffer + 11, 2);
    memcpy(&sectors_per_cluster, buffer + 13, 1);
    memcpy(&reserved_sector_count, buffer + 14, 2);
    memcpy(&table_count, buffer + 16, 1);
    memcpy(&root_entry_count, buffer + 17, 2);
    memcpy(&total_sectors_16, buffer + 19, 2);
    memcpy(&media_type, buffer + 21, 1);
    memcpy(&table_size_16, buffer + 22, 2);
    memcpy(&sectors_per_track, buffer + 24, 2);
    memcpy(&head_side_count, buffer + 26, 2);
    memcpy(&hidden_sector_count, buffer + 28, 4);
    memcpy(&total_sectors_32, buffer + 32, 4);
    memcpy(&table_size_32, buffer + 36, 4);
    memcpy(&extended_flags, buffer + 40, 2);
    memcpy(&fat_version, buffer + 42, 2);
    memcpy(&root_cluster, buffer + 44, 4);
    memcpy(&fat_info, buffer + 48, 2);
    memcpy(&backup_boot_sector, buffer + 50, 2);
    memcpy(reserved_0, buffer + 52, 12);
    memcpy(&drive_number, buffer + 64, 1);
    memcpy(&reserved_1, buffer + 65, 1);
    memcpy(&boot_signature, buffer + 66, 1);
    memcpy(&volume_id, buffer + 67, 4);
    memcpy(volume_label, buffer + 71, 11);
    memcpy(fs_type, buffer + 82, 8);
};

void BootSector::display()
{
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