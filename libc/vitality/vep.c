#include "vep.h"

int loadvep(vepheader_t vep, char* data)  {
    if(vep.signature != "VP") {
        return 1;
    }
    int size = vep.vepsize;
    char* ptr = (char*)(vep.origin);
    while(size != 0) {
        *ptr++ = *data++;
        size--;
    }
    return 0;
}

int loadvepf(vepfile_t vep) {
    return loadvep(vep.vepheader,(char*)vep.vepdata);
}