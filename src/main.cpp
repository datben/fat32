#include "../include/file.h"
#include "../include/file_system.h"
#include "../include/utils.h"

#include <iostream>

using namespace std;

int main() {
  FileSystem *file_system = new FileSystem();
  file_system->mount((char *)"./data/SSD.data");
  char *data = file_system->get_data(2);
  File *file = new File(data);
  file->display();
  file_system->unmount();

  return 0;
}