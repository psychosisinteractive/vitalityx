#include "descriptortables.h"
#include "gdt.h"
#include "interrupt.h"
#include "vitality/tty.h"

void dt_install() {
    tty_pputstring("Setting up IDT\n");
    init_idt();
    tty_pputstring("Done, setting up GDT\n");
    init_gdt();
    tty_pputstring("Done\n");
}