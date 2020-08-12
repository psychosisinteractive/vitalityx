#ifndef FDRIVE_H
#define FDRIVE_H
#include "../libc/types.h"

#define FLOPPY_144_SECTORS_PER_TRACK 18 

void lba_2_chs(uint32_t lba, uint16_t* cyl, uint16_t* head, uint16_t* sector);

#define DOR_MOTD    0x80
#define DOR_MOTC    0x40
#define DOR_MOTB    0x20
#define DOR_MOTA    0x10
#define DOR_IRQ     0x8
#define DOR_RESET   0x4
#define DOR_DSEL1   0x1
#define DOR_DSEL0   0x0

#define MSR_RQM     0x80
#define MSR_DIO     0x40
#define MSR_NDMA    0x20
#define MSR_CB      0x10
#define MSR_ACTD    0x8
#define MSR_ACTC    0x4
#define MSR_ACTB    0x2
#define MSR_ACTA    0x1

enum FloppyRegisters
{
   STATUS_REGISTER_A                = 0x3F0, // read-only
   STATUS_REGISTER_B                = 0x3F1, // read-only
   DIGITAL_OUTPUT_REGISTER          = 0x3F2,
   TAPE_DRIVE_REGISTER              = 0x3F3,
   MAIN_STATUS_REGISTER             = 0x3F4, // read-only
   DATARATE_SELECT_REGISTER         = 0x3F4, // write-only
   DATA_FIFO                        = 0x3F5,
   DIGITAL_INPUT_REGISTER           = 0x3F7, // read-only
   CONFIGURATION_CONTROL_REGISTER   = 0x3F7  // write-only
};

enum FloppyCommands
{
   READ_TRACK =                 2,	// generates IRQ6
   SPECIFY =                    3,      // * set drive parameters
   SENSE_DRIVE_STATUS =         4,
   WRITE_DATA =                 5,      // * write to the disk
   READ_DATA =                  6,      // * read from the disk
   RECALIBRATE =                7,      // * seek to cylinder 0
   SENSE_INTERRUPT =            8,      // * ack IRQ6, get status of last command
   WRITE_DELETED_DATA =         9,
   READ_ID =                    10,	// generates IRQ6
   READ_DELETED_DATA =          12,
   FORMAT_TRACK =               13,     // *
   DUMPREG =                    14,
   SEEK =                       15,     // * seek both heads to cylinder X
   VERSION =                    16,	// * used during initialization, once
   SCAN_EQUAL =                 17,
   PERPENDICULAR_MODE =         18,	// * used during initialization, once, maybe
   CONFIGURE =                  19,     // * set controller parameters
   LOCK =                       20,     // * protect controller params from a reset
   VERIFY =                     22,
   SCAN_LOW_OR_EQUAL =          25,
   SCAN_HIGH_OR_EQUAL =         29
};

struct FloppyController {

};

volatile bool floppyirq_received = false;

void floppyirq();

void resetfloppy();

#endif