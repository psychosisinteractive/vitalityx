#include "vep.h"

void loadvep(vepheader_t vep, char* data)  {
    int size = vep.vepsize;
    char* ptr = (char*)(vep.origin);
    while(size != 0) {
        *ptr++ = *data++;
        size--;
    }
}