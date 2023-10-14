#include "../include/boot_sector.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main()
{
    std::ifstream myfile;
    myfile.open("./data/SSD.data");
    char *buffer = new char[90];
    myfile.read(buffer, 90);
    BootSector boot_sector(buffer);
    delete buffer;
    boot_sector.display();

    return 0;
}