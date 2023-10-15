#ifndef DEVICE_F_H
#define DEVICE_F_H

#include "device.h"
#include "file_system.h"

class DeviceFormater {

public:
  static void format(FileSystem *sys, Device *device, int bytes_per_sector, int sectors_per_cluster,
                     int reserved_sector_count, int table_size_32);
};

#endif