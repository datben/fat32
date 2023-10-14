#ifndef DIRECTORY_H
#define DIRECTORY_H

class Directory
{
public:
    static const unsigned char BYTE_SIZE = 32;

    char name[11];
    unsigned char attributes;
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

    Directory(char *buffer);
};

#endif
