#ifndef IMPL_VX_FS_H
#define IMPL_VX_FS_H

// add the include files for your fs implementations here, also add a define for not including it
#ifndef NOFAT
#include "fat.h"
#endif

// and add your filesystem type here
typedef enum {
    Unknown,
    VXBFS,
#ifndef NOFAT
    FAT,
#endif
} FILESYSTEM_TYPES;

#endif