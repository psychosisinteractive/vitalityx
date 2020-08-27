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

#define REMAPPED_MASTER(x) x+0x20
#define REMAPPED_SLAVE(x) x+0x27

void eoi() {
    outb(0x20, 0x20);
}

void isr_handler(registers_t regs)
{
    Task* ctask = getctask();
    switch(regs.int_no) {
        case 0:
            BochsConsolePrintString("\n\nDivide By Zero Failure\n\n");
            PIC_sendEOI(regs.int_no);
            eoi();
            break;
        case 6:
            BochsConsolePrintString("\n\nInvalid Opcode Failure\n\n");
            tty_pputstring("INVALID OPCODE!!!\n\nEIP=0x");
            char* opc = "           ";
            itoa(regs.eip,opc,16);
            tty_pputstring(opc);
            tty_pputstring(" ESP=0x");
            memset(opc,0,12);
            itoa(regs.esp,opc,16);
            tty_pputstring(opc);
            for(;;) {

            }
            break;
        case 13:
            BochsConsolePrintString("\n\nGENERAL PROTECTION FAULT!!!\n\n");
            clearscreen();
            tty_pputstring("GENERAL PROTECTION FAULT!!!\n\n");
            char* errcode = "  ";
            itoa(regs.err_code,errcode,10);
            BochsConsolePrintString("Errorcode: ");
            BochsConsolePrintString(errcode);
            tty_pputstring("Error Code:");
            tty_pputstring(errcode);
            tty_pputstring("\n\nPlease restart your system, and report the error code to a VitalityX developer.\n");
            bochs_bkpt();
            for(;;) {

            }
            break;
        case 14:
            eoi();
            PIC_sendEOI(regs.int_no);
            ide_irq();
            page_fault(regs);
            for(;;) {

            }
            break;
        case 15:
            eoi();
            PIC_sendEOI(regs.int_no);
            ide_irq();
            break;
        case 62:
            runvlib(regs);
            break;
        case 63:
            eoi();
            PIC_sendEOI(regs.int_no);
            BochsConsolePrintString("\n\nVitalityX SysISR Hit\n\n");
            sysisr_handler(regs);
            break;
        case REMAPPED_MASTER(1):
            BochsConsolePrintString("k");
            keyboard_handler_main();
            PIC_sendEOI(regs.int_no);
            eoi();
            break;
        default:
            tty_pputstring("UNKNOWN SYSTEM INTERRUPT!!!\n\n");
            BochsConsolePrintString("\n\nVitalityXInterrupt\n\n");
            char* inttype = "  ";
            tty_pputstring("No. ");
            itoa(regs.int_no,inttype,10);
            BochsConsolePrintString("Interrupt: ");
            BochsConsolePrintString(inttype);
            tty_pputstring(inttype);
            tty_pputstring("\n");
            while(true) {

            }
            break;
    }
}

extern void sv13h();
extern void sv3h();

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
        case 0x06:
            switch (regs.edx)
            {
                case 0:
                    sv13h();
                    break;
                case 1:
                    sv3h();
                    break;
                default:
                    break;
            }
            break;
        default: // dont know what to do
            BochsConsolePrintString("\r\nTask ");
            BochsConsolePrintString(ctask->name);
            BochsConsolePrintString(" gave a bad sysisr.\r\n");
            break;
    }
}