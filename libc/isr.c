#include "isr.h"
#include "system.h"
#include "ext/debug.h"

void isr_handler(registers_t regs)
{
    switch(regs.int_no) {
        case 0:
            BochsConsolePrintString("\n\nDivide By Zero Failure\n\n");
            
            
        default:
            BochsConsolePrintString("\n\nVitalityXInterrupt\n\n");
    }
}