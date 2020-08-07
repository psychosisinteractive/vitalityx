#ifndef VGA_H
#define VGA_H
#include "../libc/asm.h"
#include "../libc/types.h"
#include "../libc/v86.h"

#define REGS_90x60 0b0001

// the plan for vga.h
// support Mode 13h
// support Mode 3h
// PORTS

bool vga_indexmode = false;
unsigned int vga_mode;

enum VGA_PORTS {
    vga_3c0 = 0x3c0,
    vga_3c2 = 0x3c2,
    vga_3c4 = 0x3c4,
    vga_3ce = 0x3ce,
    vga_3d4 = 0x3d4
} typedef vga_port_t;

void vga_init() {
    vga_mode = 0x3; // we should be in text mode
    inb((uint16_t)0x3DA);
    vga_indexmode = true;
}

void vga_chkmode() {
    if(vga_indexmode == false) {
        inb((uint16_t)0x3DA);
        vga_indexmode = true;
    }
}

void vga_setvar(uint8_t index, uint8_t value, vga_port_t vgaport) {
    vga_chkmode();
    if(vgaport == vga_3c0) {
        outb(0x3c0,index); // write index
        outb(0x3c0,value); // set value
    } else if(vgaport == vga_3c2) {
        outb(0x3c2,value); // dont waste an operation and/or possibly mangle 0x3c2 which only has 1 register, so no index
    } else { // 3c4, 3ce, 3d4
        outb((uint16_t)vgaport,index);
        outb((uint16_t)vgaport++,value);
    }
}

uint8_t vga_getvar(uint8_t index, vga_port_t vgaport) {
    uint8_t mode;
    vga_chkmode();
    if(vgaport == vga_3c0) {
        outb(0x3c0,index);
        mode = inb(0x3c1);
    } else {
        outb((uint16_t)vgaport,index);
        mode = inb((uint16_t)vgaport++);
    }
    inb(0x3da); // just reset it no matter what
    return mode;
}

int vga_chksetvar(uint8_t index, uint8_t value, vga_port_t vgaport) {
    vga_setvar(index,value,vgaport);
    uint8_t chk = vga_getvar(index,vgaport);
    if(chk!=value) {
        return 0;
    }
    return 1;
}

int vga_setmode(uint8_t mode, bool v86) {
    if(v86) {
        switch(mode) {
            case 0x3:
                enter_v86(0x00000500,0x00007E00,0x00000500,0x00000000);
                break;
        }
        return 0;
    }
    int return_val = 0;
    switch(mode) {
        case 0x3: // code for text mode
            return_val = vga_chksetvar(0x10,0x01,vga_3c0);
            break;
        case 0x13: // code for mode 13h (320x200 linear 256-color)
            return_val = vga_chksetvar(0x10,0x41,vga_3c0);
            return_val = vga_chksetvar(0x00,0x63,vga_3c2);
            return_val = vga_chksetvar(0x04,0x0E,vga_3c4);
            return_val = vga_chksetvar(0x05,0x40,vga_3ce);
            return_val = vga_chksetvar(0x06,0x05,vga_3ce);
            return_val = vga_chksetvar(0x17,0xA3,vga_3d4);
            break;
    }
    return return_val;
}

extern void tricks_001();
int vga_asmtricks(uint8_t trick) {
    switch(trick) {
        case REGS_90x60: // causes garbage text
            tricks_001();
            break;
    }
}



#endif