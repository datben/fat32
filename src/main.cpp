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

  file_system->mkdir((char *)"my_dir");
  file_system->touch((char *)"my_file");

  string ls = file_system->ls();
  cout << ls << endl;

  file_system->echo((char *)"my_file", (char *)"Hello World!");

  string cat = file_system->cat((char *)"my_file");
  cout << cat << endl;

  file_system->unmount();
  return 0;
}