#include "../include/file.h"
#include "../include/file_system.h"
#include "../include/utils.h"

#include <iostream>
using namespace std;

int main() {
  FileSystem *file_system = new FileSystem();
  file_system->mount((char *)"./data/SSD.data");
  file_system->format(512, 4, 2, 5);
  file_system->unmount();
  return 0;
}