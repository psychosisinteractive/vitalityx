#include "system.h"
#include "types.h"

void pokeb(uint32_t *addr, uint32_t offset, uint8_t val) {
    addr+=offset;
    *addr=val;
}
uint8_t peekb(uint8_t *addr, uint32_t offset) {
    addr+=offset;
    return *addr;
}