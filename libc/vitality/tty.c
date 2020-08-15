#include "tty.h"
#include "../types.h"
#include "../../drivers/vga.h"
#include "../ext/debug.h"
#include "../system.h"

uint16_t cursor_x = 0;
uint16_t cursor_y = 0;

void tty_putstring(char* string, int len) {
    if(len == 0) {
        len = USHRT_MAX;
    }
    char chara = *string++;
    disable_cursor();
    len += 0xb8000;
    while(chara != '\x0' | !(string < (addr_t)len)) {
        switch(chara) {
            case '\n':
                cursor_y++;
                cursor_x = 0;
                break;
            default:
                BochsConsolePrintChar(chara);
                drawchar3(chara,0x7,0x0,cursor_x,cursor_y);
                cursor_x++;
                break;
        }
        // get next character
        chara = *string++;
    }
    enable_cursor(0,15);
    update_cursor(cursor_x,cursor_y);
}

void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void update_cursor(int x, int y)
{
	uint16_t pos = y * 80 + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void drawchar3(unsigned char c, unsigned char forecolour, unsigned char backcolour, int x, int y)
{
     uint16_t attrib = (backcolour << 4) | (forecolour & 0x0F);
     volatile uint16_t * where;
     where = (volatile uint16_t *)0xB8000 + (y * 80 + x) ;
     *where = c | (attrib << 8);
}

uint16_t xoff = 0;
uint16_t yoff = 0;

void clearscreen() {
    memset(0xb8000,0,80*25);
    xoff = 0;
    yoff = 0;
    cursor_x = xoff;
    cursor_y = yoff;
    update_cursor(cursor_x,cursor_x);
}

void tty_vgaputstring(char* string, int len) {
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
        tty_putstring(string,0);
        break;
    case 0x13:
        tty_vgaputstring(string,0);
        break;
    default:
        tty_vgaputstring(string,0);
        break;
    }
}

void tty_pputstringl(char* string, int len) {
    switch (vgamode())
    {
    case 0x3:
        tty_putstring(string,len);
        break;
    case 0x13:
        tty_vgaputstring(string,len);
        break;
    default:
        tty_vgaputstring(string,len);
        break;
    }
}