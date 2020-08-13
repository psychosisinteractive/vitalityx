#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "../libc/vitality/inline.h"

///
/// Gets a keyboard scancode
///
char kb_getscancode();

/// 
/// PS/2 Data Port
/// 
#define PS2_DATAPORT 0x60
///
/// PS/2 Status Register
///
#define PS2_STATUSREG 0x64
///
/// PS/2 Command Register
///
#define PS2_COMMANDREG PS2_STATUSREG

///
/// PS/2 Initialize
///
void ps2init();

#endif