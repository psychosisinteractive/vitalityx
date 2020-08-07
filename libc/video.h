#ifndef VIDEO_H
#define VIDEO_H
#include "types.h"
#include "asm.h"
#include "system.h"


static void putpixel(unsigned char* screen, int x, int y, char color) {
    unsigned where = x*3 + y*32001;
    screen[where] = color;
}

/**TextColors enum
 * All colors that Mode 3 can support.
 */
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