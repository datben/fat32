#ifndef FILE_H
#define FILE_H

#include <string>

enum FileAttributes : char {
  READ_ONLY = 0x01,
  HIDDEN = 0x02,
  SYSTEM = 0x04,
  VOLUME_LABEL = 0x08,
  DIRECTORY = 0x10,
  ARCHIVE = 0x20,
  LONG_FILE_NAME = READ_ONLY | HIDDEN | SYSTEM | VOLUME_LABEL,
  SHORT_FILE_NAME = 0x00
};

std::string get_file_attributes_string(FileAttributes attributes);

class File {
public:
  static const unsigned char BYTE_SIZE = 32;

  char *name; // 11
  FileAttributes attributes;
  char reserved;
  char creation_time_tenths;
  short creation_time;
  short creation_date;
  short last_access_date;
  short first_cluster_high;
  short last_write_time;
  short last_write_date;
  short first_cluster_low;
  int file_size;

  File();

  File(char *buffer);

  char *serialize();

  void display();

  bool is_directory();

  int get_first_cluster();
};

#endif
