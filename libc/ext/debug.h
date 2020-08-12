#ifndef BOCHS_H
#define BOCHS_H
#include "../vitality/inline.h"

///
/// Outputs a character to the Bochs console
///
#define BochsConsolePrintChar(c) outb(0xe9, c)

///
/// Outputs a string to the Bochs console
///
void BochsConsolePrintString(char* string);

#endif