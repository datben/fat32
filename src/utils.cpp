#include "../include/utils.h"
#include <stdio.h>

void print_hex(char *buffer, int size, bool back_to_line) {
  for (int i = 0; i < size; i++) {
    printf("%02X ", (unsigned char)buffer[i]);
  }
  if (back_to_line) {
    printf("\n");
  }
}

void print_char(char *buffer, int size, bool back_to_line) {
  for (int i = 0; i < size; i++) {
    printf("%c", buffer[i]);
  }
  if (back_to_line) {
    printf("\n");
  }
}

char *read_bytes(char *&buffer, int size) {
  char *bytes = new char[size];
  for (int i = 0; i < size; i++) {
    bytes[i] = buffer[i];
  }
  buffer += size;
  return bytes;
}

namespace BigEndian {

int read_int(char *&buffer) {
  int value = 0;
  for (int i = 0; i < 4; i++) {
    value |= buffer[i] << ((3 - i) * 8);
  }
  buffer += 4;
  return value;
}
short read_short(char *&buffer) {
  short value = 0;
  for (int i = 0; i < 2; i++) {
    value |= buffer[i] << ((1 - i) * 8);
  }
  buffer += 2;
  return value;
}

char read_char(char *&buffer) {
  char value = buffer[0];
  buffer += 1;
  return value;
}

} // namespace BigEndian