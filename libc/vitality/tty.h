#ifndef TTY_H
#define TTY_H

/// 
/// Puts a string in VRAM, Text mode only
///@param string The string to put
///
void tty_putstring(char* string);
///
/// Puts a string into VRAM, Mode 13 only
///@param string The string to put
///
void tty_vgaputstring(char* string);
///
/// Puts a string into VRAM, detects preferred mode
///@param string The string to put
///
void tty_pputstring(char* string);

#endif