#ifndef MEM_H
#define MEM_H
#include "../types.h"

// memory allocator
// i hate memory

typedef union {
    struct {
        size_t size;
        unsigned is_free;
        union header *next;
    } s;
    U_ALIGN16 stub;
} header_t;

uint32_t gbrk();
void brk(size_t);
void *sbrk(size_t);
header_t *get_free_block(size_t);
void *malloc(size_t);
void free(void *);
void *calloc(size_t, size_t);
void *realloc(void *, size_t);

#endif