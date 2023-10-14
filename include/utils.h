#ifndef UTILS_H
#define UTILS_H

void print_hex(char *buffer, int size, bool back_to_line);
void print_char(char *buffer, int size, bool back_to_line);

char *read_bytes(char *&buffer, int size);

namespace BigEndian {
int read_int(char *&buffer);
short read_short(char *&buffer);
char read_char(char *&buffer);

} // namespace BigEndian

#endif