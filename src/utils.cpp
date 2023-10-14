#include "../include/utils.h"
#include <stdio.h>

void print_hex(char *buffer, int size, bool back_to_line)
{
    for (int i = 0; i < size; i++)
    {
        printf("%02X ", (unsigned char)buffer[i]);
    }
    if (back_to_line)
    {
        printf("\n");
    }
}

void print_char(char *buffer, int size, bool back_to_line)
{
    for (int i = 0; i < size; i++)
    {
        printf("%c", buffer[i]);
    }
    if (back_to_line)
    {
        printf("\n");
    }
}