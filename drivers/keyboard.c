#include "keyboard.h"
#include "../libc/types.h"
#include "../libc/vitality/inline.h"

bool rcv = false;
uint8_t lbt = 0;
void ps2init() {
    rcv = false;
    // disable devices
    outb(PS2_COMMANDREG,0xAD);  
    outb(PS2_COMMANDREG,0xA7);
    inb(PS2_DATAPORT);

}