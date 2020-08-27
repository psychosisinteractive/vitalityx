#include "vxbfs.h"
#include "../system.h"
#include "vfs.h"
#include "../../drivers/serial.h"
#include "../ext/debug.h"

file_t loadfile(vxbfs_file_t *file)
{
    /*file_t vfile;
    vfile.name = file->name;
    if(file->md) {
        vfile.data = file+sizeof(vxbfs_file_t);
    } else {
        vfile.data = file->modblock->data;
    }
    return vfile;*/ // redundant
}

bool validvxbfs(vxbfs_header_t *header) {
    bool valid = false;
    if(header->signature[0] == 'V' && header->signature[1] == 'X' && header->signature[2] == 'F' && header->signature[3] == 'S') {
        valid = true;
    }
    return valid;
}

pmfl_t* readfile(vxbfs_file_t *file) {
    bochs_bkpt();
    write_serial_string("\nreading file\n");
    vxbfs_modblock_t* mb = (vxbfs_modblock_t*)((int)file->modblock + FS_LOAD_POINT);
    byte* fptr = 0x01000000;
    int len = 0;
    pmfl_t* pmfl;
    write_serial_string("starting\n");
    while(mb->nextmodblock != 0x00) {
        byte* dp = (byte*)&mb->data;
        int i = 0;
        write_serial_string("reading mb\n");
        bochs_bkpt();
        for(i = 0; i++; i < 507) {
            write_serial(*dp);
            memcpy(fptr,dp,507);
            fptr += 507;
            len++;
        }
        write_serial_string("done\n");
        mb = mb->nextmodblock;
    }
    write_serial_string("finished\n");
    pmfl->len = len;
    pmfl->ptr = 0x01000000;
    return pmfl;
}