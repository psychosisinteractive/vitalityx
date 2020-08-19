#include "system.h"
#include "types.h"
#include "isr.h"
#include "tty.h"

void pokeb(uint32_t *addr, uint32_t offset, uint8_t val) {
    addr+=offset;
    *addr=val;
}
uint8_t peekb(uint8_t *addr, uint32_t offset) {
    addr+=offset;
    return *addr;
}
void *memset (void *dest, int val, size_t len)
{
  unsigned char *ptr = dest;
  while (len-- > 0)
    *ptr++ = val;
  return dest;
}
void* memcpy(void* dstptr, const void* srcptr, size_t size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	for (size_t i = 0; i < size; i++)
		dst[i] = src[i];
	return dstptr;
}

void sleep(int duration) {
    int time = duration;
    do {
      time--;
      __asm__("nop");
    }while(time = 0);
}

void panic(char* reason) {

}

// Task panic
void panic_task(char* reason, Registers regs) {

}

// Register panic
void panic_regs(char* reason, registers_t regs) {
  tty_pputstring("KERNEL PANIC\n");
  tty_pputstring(reason);
  tty_pputstring("\n");
  sdump(regs);
}