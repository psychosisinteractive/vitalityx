#ifndef VGA_H
#define VGA_H
#include "../libc/types.h"
#define VIDEO_SEGMENT 0xA000
#define SCREEN_WIDTH 320 // we should be in mode 13h

///
/// Puts a pixel on the screen
///@param c The color of the pixel
///@param x The X of the pixel
///@param y The Y of the pixel
///
void putpixel(uint8_t c, uint16_t x, uint16_t y);
///
/// Puts a ASCII character on the screen
///@param c The character
///@param x The X of the character
///@param y The Y of the character
///@param fgcolor The foreground color
///@param bgcolor The background color
///
void drawchar(unsigned char c, uint16_t x, uint16_t y, uint8_t fgcolor, uint8_t bgcolor);

#endif