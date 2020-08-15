#include "vlib.h"
#include "../types.h"

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

void runvlib(int id) {
    addr_t vlib = descriptor->base;
    while(id != 0) {
        vlib += sizeof(vlib_entry_t);
    }
    vlib_entry_t* sel = (vlib_entry_t*)vlib;
    func_ptr_args funcptr = sel->functionptr;
    Task* ctask = getctask();
    switchTask(&ctask->regs, &vlib_registers);
    // we can now run vlib code
    Registers ctaskregs = ctask->regs;
    void (*functionptr)(Registers ctaskregs);
    functionptr = *sel->functionptr;
    functionptr(ctask->regs);
    switchTask(&vlib_registers, &ctask->regs);
}