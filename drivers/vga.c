#include "vga.h"
#include "../libc/system.h"
#include "../libc/vitality/inline.h"
#include "../libc/vitality/mem.h"
#include "../libc/isr.h"

void putpixel(uint8_t c, uint16_t x, uint16_t y) {
    uint16_t offset = (y * resolution_y) + x;
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

typedef enum {
	Mode3h,
	Mode13h
} vgamode_t;

void setvgamode(vgamode_t vgamode) {
	registers_t regs;
	switch(vgamode) {
		case Mode3h:
			regs.eax = 0x6;
			regs.edx = 0x1;
			regs.int_no = 0x3f;
			isr_handler(regs);
			break;
		case Mode13h:
			regs.eax = 0x6;
			regs.edx = 0x0;
			regs.int_no = 0x3f;
			isr_handler(regs);
			break;
	}
}

void setfont(unsigned char *font) {
	vgafont = font;
}