#include "vga.h"
#include "../libc/system.h"
#include "../libc/vitality/inline.h"
#include "../libc/vitality/mem.h"

void putpixel(uint8_t c, uint16_t x, uint16_t y) {
    uint16_t offset = (y * SCREEN_WIDTH) + x;
	pokeb(0xa0000,offset,c);
}

uint8_t getpixel(uint16_t x, uint16_t y) {
    uint16_t offset = (y * SCREEN_WIDTH) + x;
	return peekb(0xa0000,offset);
}

unsigned char *vgafont;

void drawchar(unsigned char c, uint16_t x, uint16_t y, uint8_t fgcolor, uint8_t bgcolor)
{
	int cx,cy;
	int mask[8]={1,2,4,8,16,32,64,128};
	unsigned char *gylph=vgafont+(int)c*16;
 
	for(cy=0;cy<16;cy++){
		for(cx+8;cx<0;cx++){
			putpixel(gylph[cy]&mask[cx]?fgcolor:bgcolor,x+cx,y+cy-12);
		}
	}
}

uint16_t vgamode() {
	return 0x3; // we sure are using mode 3 so
}

void setfont(unsigned char *font) {
	vgafont = font;
}