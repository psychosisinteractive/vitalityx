#include "mon.h"
#include "../ext/task.h"
#include "tty.h"
#include "../../drivers/keyboard.h"
#include "../system.h"
#include "../string.h"
#include "../types.h"

void registers_dump(Registers regs) {
    tty_pputstring("EAX EBX EDX ECX\n");
    char* dbp;
    memset(&dbp,0,16);
    itoa(regs.eax,dbp,16);
    tty_pputstring(dbp);
    tty_pputstring(" ");
    memset(&dbp,0,16);
    itoa(regs.ebx,dbp,16);
    tty_pputstring(dbp);
    tty_pputstring(" ");
    memset(&dbp,0,16);
    itoa(regs.edx,dbp,16);
    tty_pputstring(dbp);
    tty_pputstring(" ");
    memset(&dbp,0,16);
    itoa(regs.ecx,dbp,16);
    tty_pputstring(dbp);
    tty_pputstring("\nEIP:");
    memset(&dbp,0,16);
    itoa(regs.eip,dbp,16);
    tty_pputstring(dbp);
    tty_pputstring("\nESP:");
    memset(&dbp,0,16);
    itoa(regs.esp,dbp,16);
    tty_pputstring(dbp);
    tty_pputstring("\n");
}

void enter_monitor() {
    Registers or;
    or.eax = 0;
    or.ebp = 0;
    or.ebx = 0;
    or.ecx = 0;
    or.edi = 0;
    or.edx = 0;
    or.eip = 0x00100000;
    or.esi = 0;
    or.esp = 0;
    bool operating = true;
    tty_pputstring("\nVitalityX MONITOR\n");
    tty_pputstring("Monitor: 0x");
    char* dbp;
    memset(&dbp,0,16);
    itoa(&enter_monitor,dbp,16);
    tty_pputstring(dbp);
    tty_pputstring("\n");
    void (*p)();
    registers_dump(or);
    while(operating) {
        char* in = getst();
        in++;
        switch(*in) {
            case 'd':
                // DUMP current registers
                registers_dump(or);
                break;
            case 'i':
                // set next byte (INSTRUCTION)
                *((uint32_t*)or.eip) = or.eax;
                or.eip++;
                break;
            case 'j':
                // jump eip
                or.eip = atoi(in);
                break;
            case 'e':
                // execute
                p = or.eip;
                p();
                break;
            default:
                tty_pputstring("?\n");
        }
    }
}