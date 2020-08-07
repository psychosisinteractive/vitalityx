#ifndef V86_H
#define V86_H
#include "asm.h"
#include "types.h"

/* void enter_v86
 * defined in k.asm
 * 
 */
extern void enter_v86(uint32_t ss, uint32_t esp, uint32_t cs, uint32_t eip);

#endif