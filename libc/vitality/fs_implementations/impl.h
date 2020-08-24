#ifndef IMPL_VX_FS_H
#define IMPL_VX_FS_H

// add the include files for your fs implementations here
#include "fat.h"

// and add your filesystem type here
typedef enum {
    Unknown,
    VXBFS,
    FAT,
} FILESYSTEM_TYPES;

#endif