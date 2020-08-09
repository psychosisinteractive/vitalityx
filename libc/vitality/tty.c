#include "tty.h"
#include "../types.h"

int* videoMemoryPtr = (int*)0xa000;
void tty_putstring(char* string) {
    uint16_t attrib = (0x0 << 4) | (0xf & 0x0F);
    char chara = *string++;
    while(chara != '\x0') {
        *videoMemoryPtr++ = chara;
        chara = *string++;
    }
}