#include "../include/file_system.h"

int main()
{
    FileSystem *file_system = new FileSystem();
    file_system->mount((char *)"./data/SSD.data");
    file_system->boot_sector->display();
    file_system->unmount();
    return 0;
}