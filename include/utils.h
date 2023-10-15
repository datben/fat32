#ifndef UTILS_H
#define UTILS_H

void print_hex(char *buffer, int size, bool back_to_line);
void print_char(char *buffer, int size, bool back_to_line);

char *read_bytes(char *&buffer, int size);
char read_char(char *&buffer);

void write_char(char *&buffer, char value);
void write_bytes(char *&buffer, char *value, int size);

// namespace BigEndian {
// int read_int(char *&buffer);
// short read_short(char *&buffer);

// void write_int(char *&buffer, int value);
// void write_short(char *&buffer, short value);

// } // namespace BigEndian

namespace LittleEndian {
int read_int(char *&buffer);
short read_short(char *&buffer);

void write_int(char *&buffer, int value);
void write_short(char *&buffer, short value);

} // namespace LittleEndian

#endif