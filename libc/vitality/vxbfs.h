// VitalityX Basic Filesystem
#ifndef VXBFS_H
#define VXBFS_H
#include "../types.h"
#include "vfs.h"

#define VXBFS_NOMOD 1
#define VXBFS_PROTECTED 2
#define VXBFS_KERNEL 4

///
/// This is a VXBFS Modification Block.
/// It stores 507 bytes of data (in order to fit in 1 sector), an ignore boolean and a nextmodblock address.
///
typedef struct VXBFSModificationBlock {
    char data[507];
    bool ignore;
    struct VXBFSModificationBlock *nextmodblock;
} vxbfs_modblock_t;

///
/// This is a VXBFS File.
/// It stores the 11 character name, a GUID, an ext, flags, next modblock, nextfile and a md boolean.
/// The GUID is a unique identifier so multiple VXBFS files can share the same name.
/// The ext is the VXBFS File type.
/// The md means "More Data", this signifies that there should be a search for more data after this.
/// The flags are data which is for the OS.
///
typedef struct VXBFSFile {
    char name[11];
    char guid[8];
    char ext[3];
    char flags;
    vxbfs_modblock_t *modblock;
    struct VXBFSFile *nextfile;
    bool md;
} vxbfs_file_t;

///
/// This is a VXBFS Header.
/// It stores bootcode, a label and a signature.
/// The Signature must be 'V' 'X' 'F' 'S' in order to signal a existing data partiton.
///
typedef struct VXBFSheader {
    char rvsp[4];
    char label[6];
    char uniquename[3];
    // this should be 'V' 'X' 'F' 'S'
    char signature[4]; 
    vxbfs_file_t* file;
    // 494 bytes of data for bootsector
    char bootsect[494];
} vxbfs_header_t;

file_t loadfile(vxbfs_file_t *);
bool validvxbfs(vxbfs_header_t *);

#endif