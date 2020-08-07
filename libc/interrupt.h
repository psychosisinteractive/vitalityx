#ifndef INTERRUPT_H
#define INTERRUPT_H
#include "string.h"
#include "keyboard.h"
#include "tty.h"
#include "types.h"

void isr_dividebyzero() {
    tty_serial_putstr("!interrupt \"division by zero\"");
    tty_putstr("Division by zero failure",0xc,0x4);
}

void isr_generic() {
    tty_serial_putstr("!interrupt \"generic interrupt\"");
    tty_putstr("Generic interrupt fault\r\n",0xc,0x4);
    while(1==1) {
        
    }
}

void isr_gpfault(int errorcode) {
    tty_serial_putstr("!interrupt \"protection fault\"");
    tty_putstr("General protection fault",0xc,0x4);
    char *code;
    itoa(errorcode,code,16);
    tty_putstr(code,Black,White);
    while(1==1) {

    }
}

/**Enables NMI's.
 */
void NMI_enable() {
    outb(0x70, inb(0x70) & 0x7F);
}

/**Disables NMI's.
 */
void NMI_disable() {
    outb(0x70, inb(0x70) | 0x80);
}

#endif