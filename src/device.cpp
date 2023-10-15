#include "../include/device.h"
#include <fstream>
#include <iostream>
#include <string>

Device::Device(char *path) { this->path = new std::string(path); }

void Device::read(char *buffer, int size, int offset) {
  std::ifstream file_stream;
  file_stream.open(path->c_str());
  file_stream.seekg(offset);
  file_stream.read(buffer, size);
  file_stream.close();
}

void Device::write(char *buffer, int size, int offset) {
  std::fstream file_stream;
  file_stream.open(path->c_str());
  file_stream.seekp(offset);
  file_stream.write(buffer, size);
  file_stream.close();
}