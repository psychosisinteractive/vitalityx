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
#include "../libc/vitality/mon.h"
#include "../libc/system.h"
#include "../libc/string.h"
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
    // clear the entries
    memset(&vlibe,0,sizeof(vlib_entry_t)*255);
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
    //clearscreen();
    ide_initialize(0x1F0, 0x3F6, 0x170, 0x376, 0x000);
    getentries();
    /*tty_pputstring("Select drive number from serial otherwise bfs/ata/atp0... ");
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
    tty_pputstring('\n');*/
    tty_pputstring("Loading VXBFS on ");
    tty_pputstringl((char*)(fsentries[primarydrive].name),4);
    tty_pputstring("\n");
    ide_read_sectors(primarydrive,10,0,0,0x500);
    vxbfs_header_t* header = (vxbfs_header_t*)0x500;
    if(validvxbfs(header)) {
        tty_pputstring("VXBFS found. label: ");
        tty_pputstringl(header->label,6);
        tty_pputstring("\n");
        int addr = 0x500 + (int)header->file;
        vxbfs_file_t *firstfile = (vxbfs_file_t*)addr;
        tty_pputstring("First VXBFS File: ");
        char *fname = &firstfile->name;
        tty_pputstringl(fname,11);
        tty_pputstring(".");
        char *fext = &firstfile->name;
        tty_pputstringl(fext,3);
        tty_pputstring("\n:x ");
        char *fptrs = "        ";
        itoa(addr,fptrs,16);
        tty_pputstring(fptrs);
        tty_pputstring("\n");
    }
    // test
    frconf_t* fp1 = findfile(primarydrive,"FileTest123");
    if(!fp1->found) {
        tty_pputstring("Warning: Error with findfile (Could not find FileTest123)\n");
    } else {
        tty_pputstring("Testing read");
        pmfl_t* f = readfile((vxbfs_file_t*)fp1->ptr);
        bochs_bkpt();
        // clear it
        memset(f->ptr,0,f->len);
    }
    frconf_t* fp = findfile(primarydrive,"SHELL      ");
    if(!fp->found) {
        tty_pputstring("Warning: No SHELL file\n");
    } else {
        tty_pputstring("Loading SHELL file\n");
        pmfl_t* f = readfile((vxbfs_file_t*)fp->ptr);
        tty_pputstring("Done\n");
        tty_pputstring("Size: ");
        char* fsize;
        itoa(f->len,fsize,16);
        tty_pputstring(fsize);
        tty_pputstring(" Ptr: ");
        char* fptr;
        itoa(f->ptr,fptr,16);
        tty_pputstring(fptr);
        tty_pputstring("h\n");
        // TODO: begin loading shells
    }
    // we can now enter the monitor
    enter_monitor();
    return 0;
}