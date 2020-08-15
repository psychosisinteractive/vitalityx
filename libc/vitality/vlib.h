#ifndef VLIB_H
#define VLIB_H
#include "../types.h"
#include "../ext/task.h"

typedef struct vlib_entry {
    uint16_t mode;
    bool instantiated;
    func_ptr_args functionptr;
} vlib_entry_t;

typedef struct vlibdescriptor {
    addr_t base;
    uint32_t len;
} vlibdesriptor_t;

vlibdesriptor_t *descriptor;
Registers vlib_registers;
void loadvlib(vlibdesriptor_t* d);
vlib_entry_t* getvlibentry(int id);
void runvlib(int id);

#endif