#ifndef MEM_H
#define MEM_H
#include "../types.h"

typedef struct mementry {
    bool instantiated;
    uint32_t base;
    uint32_t size;
    char purpose[16];
} mementry_t;

void setupmalloc();
uint32_t malloc(uint32_t base, uint32_t size, char *purpose);
void memfree(uint32_t memid);

void mempoke(uint32_t memid, uint32_t addr, uint8_t v);
uint8_t mempeek(uint32_t memid, uint32_t addr);
addr_t malloctoaddr(uint32_t memid, uint32_t addr);

#endif