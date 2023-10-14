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

    char *buffer = new char[BootSector::BYTE_SIZE];
    device->read(buffer, BootSector::BYTE_SIZE, 0);
    boot_sector = new BootSector(buffer);
    delete buffer;
}

void FileSystem::unmount()
{
    delete device;
    delete boot_sector;
    is_mounted = false;
}