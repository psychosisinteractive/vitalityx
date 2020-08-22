#ifndef MODEM_H
#define MODEM_H
#include "../libc/types.h"

bool acknowledge();
void alternate_mode();
bool acknowledge();
void send_modem_command();
void modem_dial(char* ph);
void modem_connect();
void getmodem();

#endif