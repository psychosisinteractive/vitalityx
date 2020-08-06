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

int kernel()
{
    tty_cleartty();
    tty_init_serial(0x3f8);
    tty_write_serial("hello serial");

    tty_cleartty();
    tty_update_cursor(0,0);
    play_sound(1000);
    tty_putstr("VITALITY\x05\xa2\x06X\r\n",Black,White);
    nosound();
    if(are_interrupts_enabled()) {
        tty_putstr("INTERRUPTS ARE ENABLED\r\n",LightGray,Black);
    }
    tty_putstr("Setting up ATA\r\n",LightGray,Black);
    ide_initialize(0x1F0, 0x3F6, 0x170, 0x376, 0x000);
    ide_read_sectors(0,5,0,0,0x7E00);
    tty_putstr("Reading FAT on Drive 0\r\n",LightGray,Black);

    tty_putstr("Reading BPB\r\n",LightGray,Black);
    struct fat_bpb bpb = *(struct fat_bpb*)((char*)0x7e00);
    tty_putstr("Getting FAT Info\r\n",LightGray,Black);
    struct fat_drvinfo fat = fat_getinfo(bpb);
    tty_putstr("Getting EBR\r\n",LightGray,Black);
    struct fat_ebr16 ebr = *fat.ebr;
    
    tty_putstr("OEM ",LightGray,Black);
    tty_putstr(bpb.OEM,LightGray,Black);
    tty_putstr("\r\n",LightGray,Black);
    tty_putstr("LABEL ",LightGray,Black);
    tty_putstr(ebr.label,LightGray,Black);
    tty_putstr("\r\n",LightGray,Black);
    char* sz;
    itoa(fat.total_sectors,sz,10);
    tty_putstr("TOTAL SECTORS ",LightGray,Black);
    tty_putstr(sz,LightGray,Black);
    tty_putstr("\r\n",LightGray,Black);
    char* szx;
    itoa(fat.total_clusters,szx,10);
    tty_putstr("TOTAL CLUSTERS ",LightGray,Black);
    tty_putstr(szx,LightGray,Black);
    tty_putstr("\r\n",LightGray,Black);
    char* szy;
    itoa(fat.fat_size,szy,10);
    tty_putstr("FAT SIZE ",LightGray,Black);
    tty_putstr(szy,LightGray,Black);
    tty_putstr("\r\n",LightGray,Black);
    char* szg;
    itoa(fat_gettype(fat),szg,10);
    tty_putstr("FAT TYPE ",LightGray,Black);
    tty_putstr(szg,LightGray,Black);
    tty_putstr("\r\n",LightGray,Black);

    bool operating = true;
    while(operating) {
        char *in = getstr();
        tty_putstr("\r\n",White,Black);
        tty_putstr(in,White,Black);
        tty_putstr("\r\n",White,Black);
    }
}