#include "../include/file.h"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

File::File(char *buffer) {
  memcpy(name, buffer, 11);
  memcpy(&attributes, buffer + 11, 1);
  memcpy(&reserved, buffer + 12, 1);
  memcpy(&creation_time_tenths, buffer + 13, 1);
  memcpy(&creation_time, buffer + 14, 2);
  memcpy(&creation_date, buffer + 16, 2);
  memcpy(&last_access_date, buffer + 18, 2);
  memcpy(&first_cluster_high, buffer + 20, 2);
  memcpy(&last_write_time, buffer + 22, 2);
  memcpy(&last_write_date, buffer + 24, 2);
  memcpy(&first_cluster_low, buffer + 26, 2);
  memcpy(&file_size, buffer + 28, 4);
};