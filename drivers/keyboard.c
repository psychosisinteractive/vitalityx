#include "keyboard.h"
#include "../libc/types.h"
#include "../libc/vitality/inline.h"
#include "pic.h"
#include "../libc/vitality/tty.h"
#include "serial.h"

bool wfk = false;
char lkey = 0;

unsigned char keyboard_map[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

#define SR_DUALCHANNEL 0b00010000
#define SR_CLEAR       0b00000010
#define SR_RESPONSE    0b00000001

void kb_init(void)
{
    // disable devices
    outb(0x64, 0xad);
    while(inb(0x64) & SR_CLEAR)
        ;
    outb(0x64, 0xa7);
    while(inb(0x64) & SR_CLEAR)
        ;
    // flush output
    inb(0x60);
    // set config
    outb(0x64, 0x20);
    while(!(inb(0x64) & SR_RESPONSE))
        ;
    uint8_t config = inb(0x60);
    bool dualchannel = false;
    if(config & SR_DUALCHANNEL)
        dualchannel = true;
    config &= ~(1 << 0);
    config &= ~(1 << 1);
    config &= ~(1 << 6);
    outb(0x64, 0x60);
    while(inb(0x64) & SR_CLEAR)
        ;
	outb(0x60, config);

    // do self test
    outb(0x64, 0xaa);
    while(!(inb(0x64) & SR_RESPONSE))
        ;
    uint8_t response = inb(0x60);
    if(response == 0x55) {
        tty_pputstring("PS/2 OK.\n");
    } else {
        tty_pputstring("PS/2 responded with 0xFC!!! The keyboard will not work.\n");
    }
    outb(0x64, 0xab);
    while(!(inb(0x64) & SR_RESPONSE))
        ;
    response = inb(0x60);
    switch(response) {
        case 0x00:
            tty_pputstring("Keyboard test passed!\n");
            break;
        case 0x01:
            tty_pputstring("Keyboard clock line struck low!\n");;
            break;
        case 0x02:
            tty_pputstring("Keyboard clock line struck high!\n");
            break;
        case 0x03:
            tty_pputstring("Keyboard data line struck low!\n");
            break;
        case 0x04:
            tty_pputstring("Keyboard data line struck high!\n");
            break;
    }
    while(inb(0x64) & SR_CLEAR)
        ;
    outb(0x64, 0xae);
    while(inb(0x64) & SR_CLEAR)
        ;
    outb(0x64, 0x60);
    while(inb(0x64) & SR_CLEAR)
        ;
    config |= 1 << 0;
	outb(0x60, config);
}

char getsch() {
    return read_serial();
}

char getch() {
    wfk = true;
    enable_cursor(0,15);
    while(wfk)
        ;
    tty_pputstring(lkey);
    disable_cursor();
    return lkey;
}

char *getst() {
    char* inch;
    char inchara = getch();
    while(inchara != '\n') {
        *inch++ = inchara;
        inchara = getch();
    }
    inch++;
    *inch = 0x0;
    return inch;
}

void keyboard_handler_main(void) {
	unsigned char status;
	char keycode;
	status = inb(KEYBOARD_STATUS_PORT);
	/* Lowest bit of status will be set if buffer is not empty */
	if (status & 0x01) {
		keycode = inb(KEYBOARD_DATA_PORT);
		if(keycode < 0)
			return;
        lkey = keycode;
        wfk = false;
	}
}
