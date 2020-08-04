#ifndef SYSTEM_H
#define SYSTEM_H

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

#endif