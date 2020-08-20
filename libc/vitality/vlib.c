#include "vlib.h"
#include "../types.h"
#include "../isr.h"

void loadvlib(vlibdesriptor_t* d) {
    descriptor = d;
}

vlib_entry_t* getvlibentry(int id) {
    addr_t vlib = descriptor->base;
    while(id != 0) {
        vlib += sizeof(vlib_entry_t);
    }
    return (vlib_entry_t*)vlib;
}

void runvlib(registers_t regs) {
    addr_t vlib = descriptor->base;
    int id = regs.ebx;
    while(id != 0) {
        vlib += sizeof(vlib_entry_t);
    }
    vlib_entry_t* sel = (vlib_entry_t*)vlib;
    func_ptr_args funcptr = sel->functionptr;
    // we can now run vlib code
    void (*functionptr)(registers_t ctaskregs);
    functionptr = *sel->functionptr;
    functionptr(regs);
}