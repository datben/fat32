#include "../include/file.h"
#include "../include/utils.h"
#include <cstring>
#include <iostream>
using namespace std;

File::File(char *buffer) {
  char *&buffer_viewer = buffer;
  name = read_bytes(buffer_viewer, 11);
  attributes = static_cast<FileAttributes>(read_char(buffer_viewer));
  reserved = read_char(buffer_viewer);
  creation_time_tenths = read_char(buffer_viewer);
  creation_time = LittleEndian::read_short(buffer_viewer);
  creation_date = LittleEndian::read_short(buffer_viewer);
  last_access_date = LittleEndian::read_short(buffer_viewer);
  first_cluster_high = LittleEndian::read_short(buffer_viewer);
  last_write_time = LittleEndian::read_short(buffer_viewer);
  last_write_date = LittleEndian::read_short(buffer_viewer);
  first_cluster_low = LittleEndian::read_short(buffer_viewer);
  file_size = LittleEndian::read_int(buffer_viewer);
};

void File::display() {
  cout << "Name: ";
  print_char(name, 11, true);
  cout << "Attributes: ";
  string enum_str = get_file_attributes_string(attributes);
  print_char((char *)enum_str.c_str(), enum_str.length(), true);
  cout << "Reserved: " << (int)reserved << endl;
  cout << "Creation Time Tenths: " << (int)creation_time_tenths << endl;
  cout << "Creation Time: " << (int)creation_time << endl;
  cout << "Creation Date: " << (int)creation_date << endl;
  cout << "Last Access Date: " << (int)last_access_date << endl;
  cout << "First Cluster High: " << (int)first_cluster_high << endl;
  cout << "Last Write Time: " << (int)last_write_time << endl;
  cout << "Last Write Date: " << (int)last_write_date << endl;
  cout << "First Cluster Low: " << (int)first_cluster_low << endl;
  cout << "File Size: " << (int)file_size << endl;
}

bool File::is_directory() { return attributes == DIRECTORY; }

int File::get_first_cluster() { return first_cluster_low | (first_cluster_high << 16); }

string get_file_attributes_string(FileAttributes attributes) {
  string result = "";
  switch (attributes) {
  case READ_ONLY:
    result += "READ_ONLY";
    break;
  case HIDDEN:
    result += "HIDDEN";
    break;
  case SYSTEM:
    result += "SYSTEM";
    break;
  case VOLUME_LABEL:
    result += "VOLUME_LABEL";
    break;
  case DIRECTORY:
    result += "DIRECTORY";
    break;
  case ARCHIVE:
    result += "ARCHIVE";
    break;
  case LONG_FILE_NAME:
    result += "LONG_FILE_NAME";
    break;
  case SHORT_FILE_NAME:
    result += "SHORT_FILE_NAME";
    break;
  }
  return result;
}