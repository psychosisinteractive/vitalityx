#include "vga.h"
#include "../libc/system.h"
#include "../libc/vitality/inline.h"

void putpixel(uint8_t c, uint16_t x, uint16_t y) {
    uint16_t offset = (y * SCREEN_WIDTH) + x;
    pokeb(VIDEO_SEGMENT,offset,c);
}

unsigned char *font;

void drawchar(unsigned char c, uint16_t x, uint16_t y, uint8_t fgcolor, uint8_t bgcolor)
{
	int cx,cy;
	int mask[8]={1,2,4,8,16,32,64,128};
	unsigned char *gylph=font+(int)c*16;
 
	for(cy=0;cy<16;cy++){
		for(cx=0;cx<8;cx++){
			putpixel(gylph[cy]&mask[cx]?fgcolor:bgcolor,x+cx,y+cy-12);
		}
	}
}