#ifndef IDE_H
#define IDE_H
#include "../libc/types.h"

struct IDEChannelRegisters {
    unsigned short base;  // I/O Base.
    unsigned short ctrl;  // Control Base
    unsigned short bmide; // Bus Master IDE
    unsigned char  nIEN;  // nIEN (No Interrupt);
} channels[2];

byte package[1];

struct ide_device {
    unsigned char  Reserved;    // 0 (Empty) or 1 (This Drive really exists).
    unsigned char  Channel;     // 0 (Primary Channel) or 1 (Secondary Channel).
    unsigned char  Drive;       // 0 (Master Drive) or 1 (Slave Drive).
    unsigned short Type;        // 0: ATA, 1:ATAPI.
    unsigned short Signature;   // Drive Signature
    unsigned short Capabilities;// Features.
    unsigned int   CommandSets; // Command Sets Supported.
    unsigned int   Size;        // Size in Sectors.
    unsigned char  Model[41];   // Model in string.
};

struct ide_device ide_devices[4];

void ide_write(unsigned char channel, unsigned char reg, unsigned char data);
unsigned char ide_read(unsigned char channel, unsigned char reg);
void ide_read_buffer(unsigned char channel, unsigned char reg, unsigned int buffer,
                     unsigned int quads);
unsigned char ide_polling(unsigned char channel, unsigned int advanced_check);
unsigned char ide_print_error(unsigned int drive, unsigned char err);
void ide_initialize(unsigned int BAR0, unsigned int BAR1, unsigned int BAR2, unsigned int BAR3,
unsigned int BAR4);
unsigned char ide_ata_access(unsigned char direction, unsigned char drive, unsigned int lba, 
                             unsigned char numsects, unsigned short selector, unsigned int edi);
void ide_read_sectors(unsigned char drive, unsigned char numsects, unsigned int lba,
                      unsigned short es, unsigned int edi);
void ide_write_sectors(unsigned char drive, unsigned char numsects, unsigned int lba,
                       unsigned short es, unsigned int edi);
void ide_atapi_eject(unsigned char drive);

void ide_irq();

void ide_wait_irq();

#endif