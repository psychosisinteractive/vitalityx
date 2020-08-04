#ifndef TTY_H
#define TTY_H
#include "string.h"
#include "asm.h"

#define TTY_TC_REVERSE "\x05\xa2\x06"
#define TTY_TC_RESET "\x05\xa1\x06"
#define TTY_TC_CL_OK "\x05\xa0\x06"

unsigned int ttylastx;
unsigned int ttylasty;
unsigned int screenheight = 25;
unsigned int port;

bool ttyserialenabled = false;

//this is the bitmap font you've loaded
/*
unsigned char *font;
 
void drawchar(unsigned char c, int x, int y, int fgcolor, int bgcolor)
{
	int cx,cy;
	int mask[8]={1,2,4,8,16,32,64,128};
	unsigned char *gylph=font+(int)c*16;
 
	for(cy=0;cy<16;cy++){
		for(cx=0;cx<8;cx++){
			putpixel(glyph[cy]&mask[cx]?fgcolor:bgcolor,x+cx,y+cy-12);
		}
	}
}*/

void tty_putchar(unsigned char c, textcolor_t forecolour, textcolor_t backcolour, int x, int y)
{
    uint16_t attrib = (backcolour << 4) | (forecolour & 0x0F);
    volatile uint16_t * where;
    where = (volatile uint16_t *)0xB8000 + (y * 80 + x) ;
    *where = c | (attrib << 8);
}

void tty_putstr(string_t string, textcolor_t forecolour, textcolor_t backcolour) {
    int ox = ttylastx;
    unsigned char forecolouro = forecolour;
    unsigned char backcolouro = backcolour;
    unsigned char ofc = 0;
    bool controlmode = false;
    while(*string != 0) {        
        if(ttylasty > screenheight) {
            ttylasty = 0;
        }
        char character = *string++;
        if(controlmode) {
            switch(character) {
                case '\xa1': // reset color
                    forecolour = forecolouro;
                    backcolour = backcolouro;
                    break;
                case '\xa0': // COLOR OK
                    forecolour = 0x0a;
                    break;
                case '\xa2': // highlight (reverse)
                    ofc = forecolour;
                    forecolour = backcolour;
                    backcolour = ofc;
                    break;
                case '\x06': // ACK, will be used as proto-ANSI stop
                    controlmode = false;
                    break;
            }
        } else {
            switch(character) {
                case '\n':
                    ttylasty++;
                    break;
                case '\r':
                    ttylastx = 0;
                    break;
                case '\x05': // ENQ, will be used as proto-ANSI initiator
                    controlmode = true;
                    break;
                default:
                    tty_putchar(character,forecolour,backcolour,ttylastx,ttylasty);
                    ttylastx++;
                    break;
            }
        }
    }
    tty_update_cursor(ttylastx,ttylasty);
}

void tty_enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void tty_disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void tty_update_cursor(int x, int y)
{
	uint16_t pos = y * 80 + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void tty_cleartty() {
    volatile uint16_t * where = (volatile uint16_t *)0xB8000;
    while(where != (volatile uint16_t *)0xB87D0) {
        *where++ = (char)00;
    }
    ttylastx = 0;
    ttylasty = 0;
}

#define PORT 0x3f8   /* COM1 */
 
void tty_init_serial(int sport) {
    port = sport;
    tty_putstr("Serial being enabled\r\n",0x7,0x0);
    outb(port + 1, 0x00);    // Disable all interrupts
    outb(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(port + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(port + 1, 0x00);    //                  (hi byte)
    outb(port + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    io_wait(); // yield till its all said and done
    tty_putstr("Serial enabled, 38400 BAUD, 8 BITS, NO PARITY, ONE STOP BIT\r\n",0x7,0x0);
    ttyserialenabled = true;
}

int tty_serial_received() {
    if(ttyserialenabled) {
        return inb(port + 5) & 1;
    }
}
 
char tty_read_serial() {
    if(ttyserialenabled) {
        while (tty_serial_received() == 0);
        
        return inb(port);
    } else {
        return 0;
    }
}

int tty_is_transmit_empty() {
    if(ttyserialenabled) {
        return inb(port + 5) & 0x20;
    } else {
        return 0;
    }
}
 
void tty_write_serial(char a) {
    if(ttyserialenabled) {
        while (tty_is_transmit_empty() == 0);
        
        outb(port,a);
    }
}

void tty_serial_putstr(string_t string) {
    if(ttyserialenabled) {
        int ox = ttylastx;
        while(*string != 0) {    
            char character = *string++;
            tty_write_serial(character);
        }
    }
}

#endif