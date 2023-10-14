#ifndef BOOT_SECTOR_H
#define BOOT_SECTOR_H

#include "device.h"

class BootSector
{
public:
    static const unsigned char BYTE_SIZE = 90;

    char jump_boot[3];
    char oem_name[8];
    unsigned short bytes_per_sector;
    unsigned char sectors_per_cluster;
    unsigned short reserved_sector_count;
    unsigned char table_count;
    unsigned short root_entry_count;
    unsigned short total_sectors_16;
    unsigned char media_type;
    unsigned short table_size_16;
    unsigned short sectors_per_track;
    unsigned short head_side_count;
    unsigned int hidden_sector_count;
    unsigned int total_sectors_32;
    unsigned int table_size_32;
    unsigned short extended_flags;
    unsigned short fat_version;
    unsigned int root_cluster;
    unsigned short fat_info;
    unsigned short backup_boot_sector;
    unsigned char reserved_0[12];
    unsigned char drive_number;
    unsigned char reserved_1;
    unsigned char boot_signature;
    unsigned int volume_id;
    char volume_label[11];
    char fs_type[8];

    BootSector();
    BootSector(char *buffer);
    BootSector(Device *device);

    void display();
    void load(char *buffer);
};

#endif