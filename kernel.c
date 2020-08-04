// kernel.c
#include "libc/types.h"
#include "libc/video.h"
#include "libc/tty.h"
#include "libc/interrupt.h"
#include "libc/asm.h"
#include "libc/sound.h"
#include "libc/keyboard.h"
#include "libc/system.h"

int kernel()
{
    tty_cleartty();
    tty_init_serial(0x3f8);
    tty_write_serial("hello serial");

    tty_cleartty();
    tty_update_cursor(0,0);
    play_sound(1000);
    tty_putstr("VITALITY\x05\xa2\x06X\r\n",Black,Red);
    nosound();

    bool operating = true;
    while(operating) {
        char *in = getstr();
        tty_putstr("\r\n",White,Black);
        tty_putstr(in,White,Black);
        tty_putstr("\r\n",White,Black);
    }
}