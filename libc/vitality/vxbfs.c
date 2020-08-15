#include "vxbfs.h"
#include "vfs.h"

file_t loadfile(vxbfs_file_t *file)
{
    file_t vfile;
    vfile.name = file->name;
    if(file->md) {
        vfile.data = file+sizeof(vxbfs_file_t);
    } else {
        vfile.data = file->modblock->data;
    }
    return vfile;
}

bool validvxbfs(vxbfs_header_t *header) {
    bool valid = false;
    if(header->signature[0] == 'V' && header->signature[1] == 'X' && header->signature[2] == 'F' && header->signature[3] == 'S') {
        valid = true;
    }
    return valid;
}