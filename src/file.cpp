#include "../include/file.h"
#include "../include/file_system.h"
#include "../include/utils.h"
#include <cstring>
#include <iostream>
using namespace std;

File::File(){};

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

char *File::serialize() {
  char *buffer = new char[File::BYTE_SIZE];
  char *ref = buffer;
  char *&buffer_viewer = ref;
  write_bytes(buffer_viewer, name, 11);
  write_char(buffer_viewer, attributes);
  write_char(buffer_viewer, reserved);
  write_char(buffer_viewer, creation_time_tenths);
  LittleEndian::write_short(buffer_viewer, creation_time);
  LittleEndian::write_short(buffer_viewer, creation_date);
  LittleEndian::write_short(buffer_viewer, last_access_date);
  LittleEndian::write_short(buffer_viewer, first_cluster_high);
  LittleEndian::write_short(buffer_viewer, last_write_time);
  LittleEndian::write_short(buffer_viewer, last_write_date);
  LittleEndian::write_short(buffer_viewer, first_cluster_low);
  LittleEndian::write_int(buffer_viewer, file_size);
  return buffer;
}

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

DirectoryIterator::DirectoryIterator(char *buffer, int size) {
  current_index = 0;
  this->size = size;
  this->buffer = buffer;
}

File DirectoryIterator::next() {
  char *file_buffer = new char[File::BYTE_SIZE];
  memcpy(file_buffer, buffer + current_index * File::BYTE_SIZE, File::BYTE_SIZE);
  current_index++;
  return File(file_buffer);
}

bool DirectoryIterator::has_next() {
  return current_index * File::BYTE_SIZE < size && buffer[current_index * File::BYTE_SIZE] != 0;
}

FileClusterIterator::FileClusterIterator(FileSystem *sys, int current_cluster_index) {
  this->address_index_iter = sys->fat32->get_file_address_index_iterator(current_cluster_index);
  this->sys = sys;
}

int FileClusterIterator::current_cluster_index() { return address_index_iter->current_address_index; }

char *FileClusterIterator::next() {
  int cluster_index = address_index_iter->next();
  return sys->read_cluster(cluster_index);
}

bool FileClusterIterator::has_next() { return address_index_iter->has_next(); }