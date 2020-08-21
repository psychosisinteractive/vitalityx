#ifndef TTY_H
#define TTY_H
#include "../types.h"

/// 
/// Puts a string in VRAM, Text mode only
///@param string The string to put
///@param len The length
///
void tty_putstring(char* string, int len);
///
/// Puts a string into VRAM, Mode 13 only
///@param string The string to put
///@param len The length
///
void tty_vgaputstring(char* string, int len);
///
/// Puts a string into VRAM, detects preferred mode
///@param string The string to put
///
void tty_pputstring(char* string);
///
/// Puts a string into VRAM, detects preferred mode, uses length
///@param string The string to put
///@param len The length
///
void tty_pputstringl(char* string, int len);
///
/// Draws a character at x and y in Mode 3
///@param c The character
///@param forecolor The foreground color
///@param backcolor The backgroud color
///@param x The x position
///@param y The y position
void drawchar3(unsigned char c, unsigned char forecolor, unsigned char backcolor, int x, int y);
///
/// Clears the console.
/// 
void clearscreen();
///
/// Sets the VGA TTY mode.
///
void set_ttyvgamode(short mode);
///
/// Disables the console mode cursor.
///
void disable_cursor();
///
/// Enables the console mode cursor.
///@param cursor_start Scanline the cursor starts at.
///@param cursor_end Scanline the cursor ends at.
///
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
///
/// Updates the cursor position.
///@param x The X position of the cursor.
///@param y The Y position of the cursor.
///
void update_cursor(int x, int y);

#endif