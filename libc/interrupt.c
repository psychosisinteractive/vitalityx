#include "interrupt.h"

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].sel = sel;
    idt[num].flags = flags;
    idt[num].base_hi = high_16(base);
    idt[num].base_lo = low_16(base);
}

void idt_install() {
    idtp.limit = (sizeof(struct idt_entry)*256) - 1;
    idtp.base = &idt;
    memset(&idt,0,sizeof(struct idt_entry)*256);
    idt_load();
}