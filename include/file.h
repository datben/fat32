#ifndef FILE_H
#define FILE_H

enum FileAttributes {
  READ_ONLY = 0x01,
  HIDDEN = 0x02,
  SYSTEM = 0x04,
  VOLUME_LABEL = 0x08,
  DIRECTORY = 0x10,
  ARCHIVE = 0x20,
  LONG_FILE_NAME = READ_ONLY | HIDDEN | SYSTEM | VOLUME_LABEL,
  SHORT_FILE_NAME = 0x00
};

class File {
public:
  static const unsigned char BYTE_SIZE = 32;

  char name[11];
  FileAttributes attributes;
  unsigned char reserved;
  unsigned char creation_time_tenths;
  unsigned short creation_time;
  unsigned short creation_date;
  unsigned short last_access_date;
  unsigned short first_cluster_high;
  unsigned short last_write_time;
  unsigned short last_write_date;
  unsigned short first_cluster_low;
  unsigned int file_size;

  File(char *buffer);
};

#endif
