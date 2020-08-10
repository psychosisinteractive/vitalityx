#ifndef INTERRUPT_H
#define INTERRUPT_H
#include "types.h"

struct idt_entry {
   uint16_t base_lo; // offset bits 0..15
   uint16_t sel; // a code segment selector in GDT or LDT
   uint8_t zero;      // unused, set to 0
   uint8_t flags; // type and attributes, see below
   uint16_t base_hi; // offset bits 16..31
} __attribute__((packed));;

struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));
 

struct idt_entry idt[256];
struct idt_ptr idtp;

///
/// (EXTERNAL ASM FUNCTION) Loads the IDT
///
extern void idt_load();

///
/// Sets an IDT gate
///
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
///
/// Installs the IDT (Interrupt Descriptor Table)
///
void idt_install();

#endif