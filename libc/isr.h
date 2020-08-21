#ifndef ISR_H
#define ISR_H
#include "types.h"

///
/// Dump interrupt registers
///@param regs The reigsters
///
void sdump(registers_t regs);
///
/// ISR handler
///
void isr_handler(registers_t regs);

#endif