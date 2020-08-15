#include "isr.h"
#include "system.h"
#include "ext/debug.h"
#include "../drivers/keyboard.h"
#include "../drivers/pic.h"
#include "ext/task.h"
#include "vitality/tty.h"
#include "string.h"
#include "vitality/vlib.h"
#include "page.h"
#include "../drivers/ide.h"

void sdump(registers_t regs) {
    tty_pputstring("\n");
    tty_pputstring("EAX: ");
    char reg[128];
    itoa(regs.eax,reg);
    tty_pputstring("\n");
    tty_pputstring("EBX: ");
    memset(&reg,0,128);
    itoa(regs.ebx,reg);
    tty_pputstring("\n");
    tty_pputstring("ECX: ");
    memset(&reg,0,128);
    itoa(regs.ecx,reg);
    tty_pputstring("\n");
    tty_pputstring("EDI: ");
    memset(&reg,0,128);
    itoa(regs.edi,reg);
    tty_pputstring("\n");
    tty_pputstring("EDX: ");
    memset(&reg,0,128);
    itoa(regs.edx,reg);
    tty_pputstring("\n");
    tty_pputstring("EIP: ");
    memset(&reg,0,128);
    itoa(regs.eip,reg);
    tty_pputstring("\n");
    tty_pputstring("ESI: ");
    memset(&reg,0,128);
    itoa(regs.esi,reg);
    tty_pputstring("\n");
    tty_pputstring("ESP: ");
    memset(&reg,0,128);
    itoa(regs.esp,reg);
    tty_pputstring("\n");
    tty_pputstring("user ESP: ");
    memset(&reg,0,128);
    itoa(regs.useresp,reg);
    tty_pputstring("\n");
    tty_pputstring("SS: ");
    memset(&reg,0,128);
    itoa(regs.ss,reg);
    tty_pputstring(" CS: ");
    memset(&reg,0,128);
    itoa(regs.cs,reg);
    tty_pputstring(" DS: ");
    memset(&reg,0,128);
    itoa(regs.ds,reg);
    tty_pputstring("\n");
    tty_pputstring("EFLAGS: ");
    memset(&reg,0,128);
    itoa(regs.eflags,reg);
}

void isr_handler(registers_t regs)
{
    Task* ctask = getctask();
    switch(regs.int_no) {
        case 0:
            BochsConsolePrintString("\n\nDivide By Zero Failure\n\n");
            sdump(regs);
            PIC_sendEOI(regs.int_no);
            break;
        case 1:
        case 32:
            PIC_sendEOI(regs.int_no);
            keyboard_handler_main();
            break;
        case 13:
            BochsConsolePrintString("\n\nGENERAL PROTECTION FAULT!!!\n\n");
            clearscreen();
            tty_pputstring("GENERAL PROTECTION FAULT!!!\n\n");
            char* errcode = "  ";
            itoa(regs.err_code,errcode);
            BochsConsolePrintString("Errorcode: ");
            BochsConsolePrintString(errcode);
            tty_pputstring("Error Code:");
            tty_pputstring(errcode);
            tty_pputstring("\n\nPlease restart your system, and report the error code and following dump to a VitalityX developer.\n");
            sdump(regs);
            bochs_bkpt();
            for(;;) {

            }
            break;
        case 14:
            PIC_sendEOI(regs.int_no);
            ide_irq();
            page_fault(regs);
            for(;;) {

            }
            break;
        case 15:
            PIC_sendEOI(regs.int_no);
            ide_irq();
            break;
        case 63:
            PIC_sendEOI(regs.int_no);
            BochsConsolePrintString("\n\nVitalityX SysISR Hit\n\n");
            sysisr_handler(regs);
            break;
        default:
            clearscreen();
            tty_pputstring("UNKNOWN SYSTEM INTERRUPT!!!\n\n");
            BochsConsolePrintString("\n\nVitalityXInterrupt\n\n");
            char* inttype = "  ";
            tty_pputstring("No. ");
            itoa(regs.int_no,inttype);
            BochsConsolePrintString("Interrupt: ");
            BochsConsolePrintString(inttype);
            tty_pputstring(inttype);
            tty_pputstring("\n");
            sdump(regs);
            break;
    }
}


void sysisr_handler(registers_t regs) {
    Task* ctask = getctask();
    // command switcher
    switch(regs.eax) {
        case 0x01: // debug out
            BochsConsolePrintString("\r\nTask ");
            BochsConsolePrintString(ctask->name);
            BochsConsolePrintString(":");
            BochsConsolePrintString((char *)regs.ebx);
            break;
        case 0x02: // console out
            tty_pputstring((char*)regs.ebx);
            break;
        case 0x03:
            BochsConsolePrintString("\r\nTask ");
            BochsConsolePrintString(ctask->name);
            BochsConsolePrintString(" is adding itself to Vlib.\r\n");
            addr_t base = descriptor->base;
            int id = 0;
            while(((vlib_entry_t*)base)->instantiated) {
                id++;
                base += sizeof(vlib_entry_t);
            }
            vlib_entry_t entry = *(vlib_entry_t*)base;
            entry.instantiated = true;
            entry.functionptr = regs.edi;
            entry.mode = regs.edx;
            regs.ebx = id;
            break;
        case 0x04:
            BochsConsolePrintString("\r\nTask ");
            BochsConsolePrintString(ctask->name);
            BochsConsolePrintString(" is yielding.\r\n");
            yield();
            break;
        case 0x05:
            runvlib(regs.ebx);
            break;
        default: // dont know what to do
            BochsConsolePrintString("\r\nTask ");
            BochsConsolePrintString(ctask->name);
            BochsConsolePrintString(" gave a bad sysisr.\r\n");
            break;
    }
}