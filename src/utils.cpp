#include "../include/utils.h"
#include <stdio.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

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

string resolve_name(string name, int len) {
  int name_len = (int)name.length();
  if (name_len < len) {
    string result = name;
    for (int i = 0; i < len - name_len; i++) {
      result += " ";
    }
    return result;
  } else if (name_len > len) {
    return name.substr(0, len);
  } else {
    return name;
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

char read_char(char *&buffer) {
  char value = buffer[0];
  buffer += 1;
  return value;
}

void write_char(char *&buffer, char value) {
  buffer[0] = value;
  buffer += 1;
}

void write_bytes(char *&buffer, char *value, int size) {
  for (int i = 0; i < size; i++) {
    buffer[i] = value[i];
  }
  buffer += size;
}

void LittleEndian::write_int(char *&buffer, int value) {
  for (int i = 0; i < 4; i++) {
    buffer[i] = (value >> (i * 8)) & 0xff;
  }
  buffer += 4;
}

void LittleEndian::write_short(char *&buffer, short value) {
  for (int i = 0; i < 2; i++) {
    buffer[i] = (value >> (i * 8)) & 0xff;
  }
  buffer += 2;
}

int LittleEndian::read_int(char *&buffer) {
  int value = 0;
  for (int i = 0; i < 4; i++) {
    value |= buffer[i] << (i * 8);
  }
  buffer += 4;
  return value;
}

short LittleEndian::read_short(char *&buffer) {
  short value = 0;
  for (int i = 0; i < 2; i++) {
    value |= buffer[i] << (i * 8);
  }
  buffer += 2;
  return value;
}

vector<string> split(const string &s, char delim) {
  vector<string> result;
  stringstream ss(s);
  string item;

  while (getline(ss, item, delim)) {
    result.push_back(item);
  }

  return result;
}