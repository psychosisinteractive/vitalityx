#ifndef VIRTUAL8086_H
#define VIRTUAL8086_H
#include "../types.h"

///
/// Enters Virtual 8086 mode
///@param ss Stack Segment
///@param esp Stack Pointer
///@param cs Code Segment
///@param eip Instruction Pointer
///
extern void enter_v86(uint32_t ss, uint32_t esp, uint32_t cs, uint32_t eip);
extern int detect_v86();

#endif 