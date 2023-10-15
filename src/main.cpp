#include "../include/file.h"
#include "../include/file_system.h"
#include "../include/utils.h"

#include <iostream>
#include <string>
using namespace std;

int main() {
  FileSystem *file_system = new FileSystem();
  file_system->mount((char *)"./data/SSD.data");
  file_system->format(512, 4, 2, 2);

  file_system->fat32->display();
  string ls = file_system->ls();
  cout << ls << endl;

  file_system->create_directory((char *)"test");

  file_system->fat32->display();

  ls = file_system->ls();
  cout << ls << endl;
  file_system->unmount();
  return 0;
}