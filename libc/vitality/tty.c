#include "tty.h"
#include "../types.h"
#include "../../drivers/vga.h"
#include "../ext/debug.h"

int* videoMemoryPtr = (int*)0x000A0000;
void tty_putstring(char* string) {
    BochsConsolePrintString(string);
    uint16_t attrib = (0x0 << 4) | (0xf & 0x0F);
    char chara = *string++;
    while(chara != '\x0') {
        *videoMemoryPtr++ = chara;
        chara = *string++;
    }
}

uint16_t xoff = 0;
uint16_t yoff = 0;

void tty_vgaputstring(char* string) {
    BochsConsolePrintString(string);
    char chara = *string++;
    while(chara != '\x0') {
        drawchar(chara,xoff,yoff+17,0xf,0x0);
        xoff += 8;
        chara = *string++;
    }
}

void tty_pputstring(char* string) {
    switch (vgamode())
    {
    case 0x3:
        tty_putstring(string);
        break;
    case 0x13:
        tty_vgaputstring(string);
        break;
    default:
        tty_vgaputstring(string);
        break;
    }
}