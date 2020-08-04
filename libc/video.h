#ifndef VIDEO_H
#define VIDEO_H
#include "types.h"
#include "asm.h"
#include "system.h"

/* putpixel (EGA,VGA) */
static void putpixel(unsigned char* screen, int x,int y, char color) {
    unsigned where = x*3 + y*32000;
    screen[where] = color;
}



enum TextColors
{
    Black,
    Blue,
    Green,
    Cyan,
    Red,
    Magenta,
    Brown,
    LightGray,
    DarkGray,
    LightBlue,
    LightGreen,
    LightCyan,
    LightRed,
    LightMagenta,
    Yellow,
    White,
} typedef textcolor_t;



#endif