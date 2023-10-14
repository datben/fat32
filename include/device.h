#ifndef DEVICE_H
#define DEVICE_H

#include <string>

class Device
{
public:
    std::string *path;

    Device(char *path);

    void read(char *buffer, int size, int offset);
    void write(char *buffer, int size, int offset);
};

#endif