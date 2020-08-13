#include "isr.h"
#include "system.h"
#include "ext/debug.h"
#include "../drivers/keyboard.h"
#include "../drivers/pic.h"
#include "ext/task.h"
#include "vitality/tty.h"
#include "string.h"

void isr_handler(registers_t regs)
{
    switch(regs.int_no) {
        case 0:
            BochsConsolePrintString("\n\nDivide By Zero Failure\n\n");
            PIC_sendEOI(regs.int_no);
            break;
        case 1:
            PIC_sendEOI(regs.int_no);
            break;
        case 13:
            BochsConsolePrintString("\n\nGENERAL PROTECTION FAULT!!!\n\n");
            char* errcode = "  ";
            itoa(regs.err_code,errcode);
            BochsConsolePrintString("Errorcode: ");
            BochsConsolePrintString(errcode);
            bochs_bkpt();
            for(;;) {

            }
            break;
        case 63:
            PIC_sendEOI(regs.int_no);
            BochsConsolePrintString("\n\nVitalityX SysISR Hit\n\n");
            sysisr_handler(regs);
            break;
        default:
            BochsConsolePrintString("\n\nVitalityXInterrupt\n\n");
            char* inttype = "  ";
            itoa(regs.int_no,inttype);
            BochsConsolePrintString("Interrupt: ");
            BochsConsolePrintString(inttype);
            for(;;) {

            }
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
        default: // dont know what to do
            BochsConsolePrintString("\r\nTask ");
            BochsConsolePrintString(ctask->name);
            BochsConsolePrintString(" gave a bad sysisr.\r\n");
            break;
    }
}