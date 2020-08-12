#include "fdrive.h"
#include "../libc/types.h"
#include "../libc/vitality/inline.h"

void lba_2_chs(uint32_t lba, uint16_t* cyl, uint16_t* head, uint16_t* sector)
{
    *cyl    = lba / (2 * FLOPPY_144_SECTORS_PER_TRACK);
    *head   = ((lba % (2 * FLOPPY_144_SECTORS_PER_TRACK)) / FLOPPY_144_SECTORS_PER_TRACK);
    *sector = ((lba % (2 * FLOPPY_144_SECTORS_PER_TRACK)) % FLOPPY_144_SECTORS_PER_TRACK + 1);
}

void floppyirq() {
    floppyirq_received = true;
}

void resetfloppy() {
    floppyirq_received = false;

    outb(DIGITAL_OUTPUT_REGISTER,DOR_RESET);
}

void speedfloppymtr() {
    
}