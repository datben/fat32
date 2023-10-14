#include "../include/file_system.h"
#include "../include/utils.h"

#include <iostream>

using namespace std;

int main() {
  FileSystem *file_system = new FileSystem();
  file_system->mount((char *)"./data/SSD.data");
  file_system->boot_sector->display();
  file_system->fat32->display();
  char *data = file_system->get_data(2);
  file_system->unmount();

  return 0;
}