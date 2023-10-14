#include "../include/file_system.h"
#include "../include/device.h"

FileSystem::FileSystem()
{
    is_mounted = false;
}

void FileSystem::mount(char *path)
{
    device = new Device(path);
    is_mounted = true;

    boot_sector = new BootSector(device);
}

void FileSystem::unmount()
{
    delete device;
    delete boot_sector;
    is_mounted = false;
}