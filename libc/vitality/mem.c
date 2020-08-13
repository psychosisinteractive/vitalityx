#include "mem.h"
#include "../types.h"
#include "../system.h"
#include "../ext/debug.h"
#include "inline.h"
#include "../string.h"

#define MEMENTRY_SIZE 255

mementry_t mementries[MEMENTRY_SIZE];

void setupmalloc() {
    BochsConsolePrintString("\nSetting up memory allocator\n");
    memset(&mementries,0,sizeof(mementry_t)*MEMENTRY_SIZE);
    malloc(0x01000000,255,"vitality-system");
    malloc(0x000A0000,64000,"graphic-display");
    BochsConsolePrintString("\nMementries located at ");
    char mementry[50];
    memset(&mementry,0,50);
    itoa(&mementries,mementry);
    BochsConsolePrintString(mementry);
    BochsConsolePrintString("\n");
}

uint32_t malloc(uint32_t base, uint32_t size, char *purpose) {
    BochsConsolePrintString("\nNew memory allocation being made\n");
    BochsConsolePrintString("Purpose: ");
    BochsConsolePrintString(purpose);
    BochsConsolePrintString("\n");
    int i = 0;
    bool mallocwork = false;
    for(i = 0; i < MEMENTRY_SIZE; i++) {
        if(!mementries[i].instantiated) {
            mallocwork = true;
            break;
        } else {
            continue;
        }
    }
    if(!mallocwork) {
        return 0; // nothing
    }
    mementry_t entry;

    memcpy(&entry.purpose,purpose,16);
    memset(base,0x00,size);
    entry.instantiated = true;
    entry.base = base;
    entry.size = size;
    mementries[i] = entry;
    return i;
}

void memfree(uint32_t memid) {
    
}

addr_t malloctoaddr(uint32_t memid, uint32_t addr) {
    addr += mementries[memid].base;
    return (addr_t)addr;
}

void mempoke(uint32_t memid, uint32_t addr, uint8_t v) {
    addr_t maddr = malloctoaddr(memid,addr);
    *maddr = v;
}

uint8_t mempeek(uint32_t memid, uint32_t addr) {
    addr_t maddr = malloctoaddr(memid,addr);
    return *maddr;
}