#include "gdt.h"
#include "vitality/inline.h"
#include "types.h"
#include "gdt.h"
#include "page.h"
#include "ext/debug.h"

///
/// Loads the GDT (Global Descriptor Table)
///
void init_gdt()
{
    BochsConsolePrintString("\nSetting up GDT\n");
    gdt_ptr.limit = (sizeof(gdt_entry_t) * ENTRY_NUM) - 1;
    gdt_ptr.base  = (uint32_t)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    // kernel
    BochsConsolePrintString("\nSetting up Kernel gates\n");
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    BochsConsolePrintString("\nDone\n");
    // user mode
    BochsConsolePrintString("\nSetting up User gates\n");
    gdt_set_gate(3, 0x00500000, 0x009FFFFF, 0xFA, 0xCF); // User mode code segment
    gdt_set_gate(4, 0x00500000, 0x009FFFFF, 0xF2, 0xCF); // User mode data segment
    BochsConsolePrintString("\nDone, flushing\n");

    gdt_flush((uint32_t)&gdt_ptr);
}

///
/// Set the value of one GDT entry.
///
void gdt_set_gate(sint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
   gdt_entries[num].base_low    = (base & 0xFFFF);
   gdt_entries[num].base_middle = (base >> 16) & 0xFF;
   gdt_entries[num].base_high   = (base >> 24) & 0xFF;

   gdt_entries[num].limit_low   = (limit & 0xFFFF);
   gdt_entries[num].granularity = (limit >> 16) & 0x0F;

   gdt_entries[num].granularity |= gran & 0xF0;
   gdt_entries[num].access      = access;
}
