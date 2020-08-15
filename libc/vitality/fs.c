#include "fs.h"
#include "../../drivers/ide.h"
#include "../system.h"
#include "tty.h"
#include "../string.h"
#include "vxbfs.h"

const char* nullfs = "null";

void getentries() {
    int done = 0;
    done += getentry(0);
    done += getentry(1);
    done += getentry(2);
    done += getentry(3);
    tty_pputstring("Setup ");
    char* numsetup;
    itoa(done,numsetup);
    tty_pputstring(numsetup);
    tty_pputstring(" drive(s).\n");
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
        if(ide_devices[id].Type == 0) {
            ide_read_sectors(id,4,0,0,0x500);
            if(validvxbfs((vxbfs_header_t*)0x500)) {
                vxbfs_header_t* header = (vxbfs_header_t*)0x500;
                memcpy(cnam,header->uniquename,3);
            }
        }
        memcpy(cnam+3,num,1);
        memcpy(entry->name,&cnam,4);
        tty_pputstring("Registered ");
        tty_pputstring(cnam);
        tty_pputstring(" Model ");
        tty_pputstring((char*)ide_devices[id].Model);
        tty_pputstring("\n");
        return 1;
    } else {
        fsentries[id].ready = false;
        fsentries[id].dev = 0;

        memcpy(&fsentries[id].name,&nullfs,4);
        return 0;
    } 
}