#include "fs.h"
#include "../../drivers/ide.h"
#include "../system.h"
#include "tty.h"
#include "../string.h"
#include "vxbfs.h"
// add filesystem implementations here
#include "fs_implementations/impl.h";

const char* nullfs = "null";

void getentries() {
    getentry(0);
    getentry(1);
    getentry(2);
    getentry(3);
}

int getentry(int id) {
    char* cnam;
    if(ide_devices[id].Reserved) {
        fsentry_t *entry = &fsentries[id];
        entry->ready = true;
        entry->dev = &ide_devices[id];
        
        char* num = "x";
        itoa(id,num);
        switch(ide_devices[id].Type) {
            case 0: // ata
                cnam = "ataX";
                break;
            case 1: // atapi
                cnam = "atpX";
                break;
        }
        // Put all your FS detectors below
        if(ide_devices[id].Type == 0) {
            ide_read_sectors(id,4,0,0,0x500);
            if(validvxbfs((vxbfs_header_t*)0x500)) {
                vxbfs_header_t* header = (vxbfs_header_t*)0x500;
                memcpy(cnam,header->uniquename,3);
            }
            // define this if you want no custom filesystems and just use VXBFS
            #ifndef NOCFS
            #pragma region Non VXBFS Detectors

            // ADD ANY FILESYSTEM DETECTORS IN THIS REGION

            #ifndef NOFAT
            // FAT detector -- nougatchi
            fat_BS_t* fbs = (fat_BS_t*)0x500;
            fat_descriptor_t *desc = fat_gendescriptor(fbs);
            switch(desc->fat_type) {
                case FAT12:
                case FAT16:
                case FAT32:
                    entry->mode = desc->fat_type;
                    entry->dp   = (uint32_t*)desc;
                    break;
                default:
                    if(desc->total_sectors == 0) {
                        break;
                    }
                    // its either exfat or just nothing
                    break;
            }
            #endif

            #pragma endregion
            #endif
        }
        memcpy(cnam+3,num,1);
        memcpy(entry->name,&cnam,4);
        tty_pputstring("Registered ");
        tty_pputstringl(cnam,3);
        tty_pputstring(" Model ");
        tty_pputstringl(&ide_devices[id].Model,11);
        tty_pputstring("\n");
        return 1;
    } else {
        fsentries[id].ready = false;
        fsentries[id].dev = 0;

        memcpy(&fsentries[id].name,&nullfs,4);
        return 0;
    } 
}