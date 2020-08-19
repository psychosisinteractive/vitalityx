#ifndef VGA_H
#define VGA_H
#include "../libc/types.h"
#define SCREEN_WIDTH 320 // we should be in mode 13h

///
/// Puts a pixel on the screen
///@param c The color of the pixel
///@param x The X of the pixel
///@param y The Y of the pixel
///
void putpixel(uint8_t c, uint16_t x, uint16_t y);
///
/// Gets a pixel on the screen
///@param x The X of the pixel
///@param y The Y of the pixel
///
uint8_t getpixel(uint16_t x, uint16_t y);
///
/// Puts a ASCII character on the screen
///@param c The character
///@param x The X of the character
///@param y The Y of the character
///@param fgcolor The foreground color
///@param bgcolor The background color
///
void drawchar(unsigned char c, uint16_t x, uint16_t y, uint8_t fgcolor, uint8_t bgcolor);
uint16_t vgamode = 0x3;
///
/// Sets the font
///
void setfont(unsigned char *font);
///
/// X resolution of the screen
///
uint32_t resolution_x;
///
/// Y resolution of the screen
///
uint32_t resolution_y;

#endif