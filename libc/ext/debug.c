#include "debug.h"

void BochsConsolePrintString(char* string)
{
    char chara = *string++;
    while(chara != 0x0) {
        BochsConsolePrintChar(chara);
        chara = *string++;
    }
}