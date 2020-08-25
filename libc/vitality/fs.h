#ifndef FS_H
#define FS_H
#include "../../drivers/ide.h"
#include "../system.h"
#include "tty.h"
#include "../string.h"
#include "vxbfs.h"
#include "vfs.h"
#include "fs_implementations/impl.h"

typedef struct fsentry {
    bool ready;
    char name[4];
    struct ide_device *dev;
    uint8_t mode;
    uint32_t *dp;
    FILESYSTEM_TYPES type;
} fsentry_t;

fsentry_t fsentries[4];

void getentries();

int getentry(int id);

frconf_t* findfile(int id, char* name);

#endif