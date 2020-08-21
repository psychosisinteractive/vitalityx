#ifndef KEYBOARD_H
#define KEYBOARD_H
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

#include "../libc/vitality/inline.h"

void kb_init(void);
char *getst();
char getch();
char getsch();
void keyboard_handler_main();

#endif