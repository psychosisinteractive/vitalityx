#include "../libc/vitality/tty.h"
#include "../libc/descriptortables.h"
#include "../libc/page.h"
#include "../libc/ext/debug.h"
#include "../drivers/vga.h"
#include "../libc/ext/task.h"
#include "../libc/vitality/mem.h"
#include "../libc/vitality/vlib.h"
#include "../drivers/ide.h"
#include "../libc/vitality/fs.h"
#include "../libc/vitality/vxbfs.h"
#include "../libc/vitality/vfs.h"
#include "../drivers/keyboard.h"
#include "../drivers/serial.h"
///
/// The kernels C entry point
///
vlib_entry_t vlibe[255];
vlibdesriptor_t vlibd;
int primarydrive;
int kernel() {
    tty_pputstring("VitalityX\n");
    //initialise_paging();
    tty_pputstring("Enabling keyboard\n");
    kb_init();
    tty_pputstring("Setting up new descriptor tables\n");
    dt_install();
    vlibd.base = &vlibe;
    vlibd.len = 255;
    tty_pputstring("Setting up serial interface\n");
    init_serial();
    set_serialmode(true);
    write_serial('H');
    write_serial('I');
    write_serial('\n');
    tty_pputstring("Loading VLib\n");
    loadvlib(&vlibd);
    tty_pputstring("Starting multitasking\n");
    initTasking();
    tty_pputstring("Setting up FS\n");
    ide_initialize(0x1F0, 0x3F6, 0x170, 0x376, 0x000);
    clearscreen();
    getentries();
    tty_pputstring("Select drive number from serial otherwise bfs/ata/atp0... ");
    switch(getsch()) {
        case '0':
            primarydrive = 0;
        case '1':
            primarydrive = 1;
        case '2':
            primarydrive = 2;
        case '3':
            primarydrive = 4;
        default:
            primarydrive = 0;
    }
    tty_pputstring('\n');
    tty_pputstring("Loading VXBFS on ");
    tty_pputstringl((char*)fsentries[primarydrive].name,4);
    tty_pputstring("\n");
    ide_read_sectors(primarydrive,4,0,0,0x500);
    vxbfs_header_t header = *(vxbfs_header_t*)0x500;
    if(header.signature[0] == 'V' && header.signature[1] == 'X' && header.signature[2] == 'F' && header.signature[3] == 'S') {
        tty_pputstring("VXBFS found.");
        tty_pputstring(" label: ");
        tty_pputstringl(header.label,6);
        vxbfs_file_t *firstfile = header.file;
        tty_pputstring(" First VXBDS File: ");
        tty_pputstringl((char*)firstfile->name,11);
    }
    while(true) {
        getst();
    }
    return 0;
}