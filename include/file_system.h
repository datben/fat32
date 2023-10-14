#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "device.h"
#include "boot_sector.h"

class FileSystem
{
public:
    Device *device;
    BootSector *boot_sector;

    bool is_mounted;

    FileSystem();

    void mount(char *path);
    void unmount();
};

#endif