// kernel.c
#include "libc/types.h"
#include "libc/video.h"
#include "libc/tty.h"
#include "libc/interrupt.h"
#include "libc/asm.h"
#include "libc/sound.h"
#include "libc/keyboard.h"
#include "libc/system.h"
#include "drivers/ide.h"
#include "drivers/fat.h"
#include "drivers/vga.h"

/** void kernel()
 * 
 * Entry point for VitalityX
 */
void kernel()
{
    //tty_cleartty();
    tty_init_serial(0x3f8);
    tty_write_serial("hello serial");

    //tty_cleartty();
    tty_update_cursor(0,0);
    play_sound(1000);
    nosound();
    if(are_interrupts_enabled()) {
        tty_putstr("INTERRUPTS ARE ENABLED\r\n",LightGray,Black);
    }

    vga_setmode(0x13,true);
    

    tty_putstr("VITALITY\x05\xa2\x06X\r\n",Black,White);

}