#include "descriptortables.h"
#include "gdt.h"
#include "interrupt.h"

void dt_install() {
    init_idt();
    init_gdt();
}