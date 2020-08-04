#ifndef SYSTEM_H
#define SYSTEM_H
#include "asm.h"

extern void setup_isr();

void setup_descriptors() {
    setup_isr();
}

void memset(void *destination, int c, unsigned int target_size)
{
    if(target_size)
    {
        char *dst = (char*)destination;
        do {
            *dst++ = c;
        } while(--target_size);
    }
}

void sleep(int duration) {
    do{
        duration--;
    }while(duration != 0);
}

void insl(unsigned reg, unsigned int *buffer, int quads)
{
    int index;
    for(index = 0; index < quads; index++)
    {
        buffer[index] = inl(reg);
    }
}


#endif