#include "tty.h"
#include "../types.h"
#include "../../drivers/vga.h"

int* videoMemoryPtr = (int*)0xa000;
void tty_putstring(char* string) {
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
    char chara = *string++;
    while(chara != '\x0') {
        drawchar(chara,xoff,yoff+1,0xf,0x0);
        chara = *string++;
    }
}