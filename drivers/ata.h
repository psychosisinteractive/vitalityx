#ifndef ATA_H
#define ATA_H
#include "ide.h"
unsigned char ide_ata_access(unsigned char direction, unsigned char drive, unsigned int lba, 
                             unsigned char numsects, unsigned short selector, unsigned int edi) {
    unsigned char lba_mode /* 0: CHS, 1:LBA28, 2: LBA48 */, dma /* 0: No DMA, 1: DMA */, cmd;
    unsigned char lba_io[6];
    unsigned int  channel      = ide_devices[drive].Channel; // Read the Channel.
    unsigned int  slavebit      = ide_devices[drive].Drive; // Read the Drive [Master/Slave]
    unsigned int  bus = channels[channel].base; // Bus Base, like 0x1F0 which is also data port.
    unsigned int  words      = 256; // Almost every ATA drive has a sector-size of 512-byte.
    unsigned short cyl, i;
    unsigned char head, sect, err;
    ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN = (ide_irq_invoked = 0x0) + 0x02);
        // (I) Select one from LBA28, LBA48 or CHS;
    if (lba >= 0x10000000) { // Sure Drive should support LBA in this case, or you are
                                // giving a wrong LBA.
        // LBA48:
        lba_mode  = 2;
        lba_io[0] = (lba & 0x000000FF) >> 0;
        lba_io[1] = (lba & 0x0000FF00) >> 8;
        lba_io[2] = (lba & 0x00FF0000) >> 16;
        lba_io[3] = (lba & 0xFF000000) >> 24;
        lba_io[4] = 0; // LBA28 is integer, so 32-bits are enough to access 2TB.
        lba_io[5] = 0; // LBA28 is integer, so 32-bits are enough to access 2TB.
        head      = 0; // Lower 4-bits of HDDEVSEL are not used here.
    } else if (ide_devices[drive].Capabilities & 0x200)  { // Drive supports LBA?
        // LBA28:
        lba_mode  = 1;
        lba_io[0] = (lba & 0x00000FF) >> 0;
        lba_io[1] = (lba & 0x000FF00) >> 8;
        lba_io[2] = (lba & 0x0FF0000) >> 16;
        lba_io[3] = 0; // These Registers are not used here.
        lba_io[4] = 0; // These Registers are not used here.
        lba_io[5] = 0; // These Registers are not used here.
        head      = (lba & 0xF000000) >> 24;
    } else {
        // CHS:
        lba_mode  = 0;
        sect      = (lba % 63) + 1;
        cyl       = (lba + 1  - sect) / (16 * 63);
        lba_io[0] = sect;
        lba_io[1] = (cyl >> 0) & 0xFF;
        lba_io[2] = (cyl >> 8) & 0xFF;
        lba_io[3] = 0;
        lba_io[4] = 0;
        lba_io[5] = 0;
        head      = (lba + 1  - sect) % (16 * 63) / (63); // Head number is written to HDDEVSEL lower 4-bits.
    }
    // (II) See if drive supports DMA or not;
    dma = 0; // We don't support DMA
    // (III) Wait if the drive is busy;
    while (ide_read(channel, ATA_REG_STATUS) & ATA_SR_BSY)
        ; // Wait if busy.
    // (IV) Select Drive from the controller;
    if (lba_mode == 0)
        ide_write(channel, ATA_REG_HDDEVSEL, 0xA0 | (slavebit << 4) | head); // Drive & CHS.
    else
        ide_write(channel, ATA_REG_HDDEVSEL, 0xE0 | (slavebit << 4) | head); // Drive & LBA
       // (V) Write Parameters;
    if (lba_mode == 2) {
        ide_write(channel, ATA_REG_SECCOUNT1,   0);
        ide_write(channel, ATA_REG_LBA3,   lba_io[3]);
        ide_write(channel, ATA_REG_LBA4,   lba_io[4]);
        ide_write(channel, ATA_REG_LBA5,   lba_io[5]);
    }
    ide_write(channel, ATA_REG_SECCOUNT0,   numsects);
    ide_write(channel, ATA_REG_LBA0,   lba_io[0]);
    ide_write(channel, ATA_REG_LBA1,   lba_io[1]);
    ide_write(channel, ATA_REG_LBA2,   lba_io[2]);
    // (VI) Select the command and send it;
    // Routine that is followed:
    // If ( DMA & LBA48)   DO_DMA_EXT;
    // If ( DMA & LBA28)   DO_DMA_LBA;
    // If ( DMA & LBA28)   DO_DMA_CHS;
    // If (!DMA & LBA48)   DO_PIO_EXT;
    // If (!DMA & LBA28)   DO_PIO_LBA;
    // If (!DMA & !LBA#)   DO_PIO_CHS;
    if (lba_mode == 0 && dma == 0 && direction == 0) cmd = ATA_CMD_READ_PIO;
    if (lba_mode == 1 && dma == 0 && direction == 0) cmd = ATA_CMD_READ_PIO;   
    if (lba_mode == 2 && dma == 0 && direction == 0) cmd = ATA_CMD_READ_PIO_EXT;   
    if (lba_mode == 0 && dma == 1 && direction == 0) cmd = ATA_CMD_READ_DMA;
    if (lba_mode == 1 && dma == 1 && direction == 0) cmd = ATA_CMD_READ_DMA;
    if (lba_mode == 2 && dma == 1 && direction == 0) cmd = ATA_CMD_READ_DMA_EXT;
    if (lba_mode == 0 && dma == 0 && direction == 1) cmd = ATA_CMD_WRITE_PIO;
    if (lba_mode == 1 && dma == 0 && direction == 1) cmd = ATA_CMD_WRITE_PIO;
    if (lba_mode == 2 && dma == 0 && direction == 1) cmd = ATA_CMD_WRITE_PIO_EXT;
    if (lba_mode == 0 && dma == 1 && direction == 1) cmd = ATA_CMD_WRITE_DMA;
    if (lba_mode == 1 && dma == 1 && direction == 1) cmd = ATA_CMD_WRITE_DMA;
    if (lba_mode == 2 && dma == 1 && direction == 1) cmd = ATA_CMD_WRITE_DMA_EXT;
    ide_write(channel, ATA_REG_COMMAND, cmd);               // Send the Command.
    if (dma)
        if (direction == 0);
            // DMA Read.
        else;
            // DMA Write.
    else
        if (direction == 0)
            // PIO Read.
        for (i = 0; i < numsects; i++) {
            if (err = ide_polling(channel, 1))
                return err; // Polling, set error and exit if there is.
            asm("pushw %es");
            asm("mov %%ax, %%es" : : "a"(selector));
            asm("rep insw" : : "c"(words), "d"(bus), "D"(edi)); // Receive Data.
            asm("popw %es");
            edi += (words*2);
        } else {
        // PIO Write.
            for (i = 0; i < numsects; i++) {
                ide_polling(channel, 0); // Polling.
                asm("pushw %ds");
                asm("mov %%ax, %%ds"::"a"(selector));
                asm("rep outsw"::"c"(words), "d"(bus), "S"(edi)); // Send Data
                asm("popw %ds");
                edi += (words*2);
            }
            ide_write(channel, ATA_REG_COMMAND, (char []) {   ATA_CMD_CACHE_FLUSH,
                            ATA_CMD_CACHE_FLUSH,
                            ATA_CMD_CACHE_FLUSH_EXT}[lba_mode]);
            ide_polling(channel, 0); // Polling.
        }
    
    return 0; // Easy, isn't it?
}

void ide_wait_irq() {
   while (!ide_irq_invoked)
      ;
   ide_irq_invoked = 0;
}

void ide_irq() {
   ide_irq_invoked = 1;
}

unsigned char ide_atapi_read(unsigned char drive, unsigned int lba, unsigned char numsects,
          unsigned short selector, unsigned int edi) {
    unsigned int   channel  = ide_devices[drive].Channel;
    unsigned int   slavebit = ide_devices[drive].Drive;
    unsigned int   bus      = channels[channel].base;
    unsigned int   words    = 1024; // Sector Size. ATAPI drives have a sector size of 2048 bytes.
    unsigned char  err;
    int i;
    // Enable IRQs:
    ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN = ide_irq_invoked = 0x0);

    // (I): Setup SCSI Packet:
    // ------------------------------------------------------------------
    atapi_packet[ 0] = ATAPI_CMD_READ;
    atapi_packet[ 1] = 0x0;
    atapi_packet[ 2] = (lba >> 24) & 0xFF;
    atapi_packet[ 3] = (lba >> 16) & 0xFF;
    atapi_packet[ 4] = (lba >> 8) & 0xFF;
    atapi_packet[ 5] = (lba >> 0) & 0xFF;
    atapi_packet[ 6] = 0x0;
    atapi_packet[ 7] = 0x0;
    atapi_packet[ 8] = 0x0;
    atapi_packet[ 9] = numsects;
    atapi_packet[10] = 0x0;
    atapi_packet[11] = 0x0;

    // (II): Select the drive:
    // ------------------------------------------------------------------
    ide_write(channel, ATA_REG_HDDEVSEL, slavebit << 4);
    // (III): Delay 400 nanoseconds for select to complete:
    // ------------------------------------------------------------------
    for(int i = 0; i < 4; i++)
        ide_read(channel, ATA_REG_ALTSTATUS); // Reading the Alternate Status port wastes 100ns.
    // (IV): Inform the Controller that we use PIO mode:
    // ------------------------------------------------------------------
    ide_write(channel, ATA_REG_FEATURES, 0);         // PIO mode.
    // (V): Tell the Controller the size of buffer:
    // ------------------------------------------------------------------
    ide_write(channel, ATA_REG_LBA1, (words * 2) & 0xFF);   // Lower Byte of Sector Size.
    ide_write(channel, ATA_REG_LBA2, (words * 2) >> 8);   // Upper Byte of Sector Size.
    // (VI): Send the Packet Command:
    // ------------------------------------------------------------------
    ide_write(channel, ATA_REG_COMMAND, ATA_CMD_PACKET);      // Send the Command.
    
    // (VII): Waiting for the driver to finish or return an error code:
    // ------------------------------------------------------------------
    if (err = ide_polling(channel, 1)) return err;         // Polling and return if error.
    
    // (VIII): Sending the packet data:
    // ------------------------------------------------------------------
    asm("rep   outsw" : : "c"(6), "d"(bus), "S"(atapi_packet));   // Send Packet Data
    // (IX): Receiving Data:
    // ------------------------------------------------------------------
    for (i = 0; i < numsects; i++) {
        ide_wait_irq();                  // Wait for an IRQ.
        if (err = ide_polling(channel, 1))
            return err;      // Polling and return if error.
        asm("pushw %es");
        asm("mov %%ax, %%es"::"a"(selector));
        asm("rep insw"::"c"(words), "d"(bus), "D"(edi));// Receive Data.
        asm("popw %es");
        edi += (words * 2);
    }
    // (X): Waiting for an IRQ:
    // ------------------------------------------------------------------
    ide_wait_irq();
    
    // (XI): Waiting for BSY & DRQ to clear:
    // ------------------------------------------------------------------
    while (ide_read(channel, ATA_REG_STATUS) & (ATA_SR_BSY | ATA_SR_DRQ))
        ;
    
    return 0; // Easy, ... Isn't it?
}

void ide_read_sectors(unsigned char drive, unsigned char numsects, unsigned int lba,
                      unsigned short es, unsigned int edi) {
 
    // 1: Check if the drive presents:
    // ==================================
    if (drive > 3 || ide_devices[drive].Reserved == 0) package[0] = 0x1;      // Drive Not Found!
    
    // 2: Check if inputs are valid:
    // ==================================
    else if (((lba + numsects) > ide_devices[drive].Size) && (ide_devices[drive].Type == IDE_ATA))
        package[0] = 0x2;                     // Seeking to invalid position.
    
    // 3: Read in PIO Mode through Polling & IRQs:
    // ============================================
    else {
        int i;
        unsigned char err;
        if (ide_devices[drive].Type == IDE_ATA)
            err = ide_ata_access(ATA_READ, drive, lba, numsects, es, edi);
        else if (ide_devices[drive].Type == IDE_ATAPI)
            for (i = 0; i < numsects; i++)
                err = ide_atapi_read(drive, lba + i, 1, es, edi + (i*2048));
        package[0] = ide_print_error(drive, err);
    }
}
// package[0] is an entry of an array. It contains the Error Code.
void ide_write_sectors(unsigned char drive, unsigned char numsects, unsigned int lba,
                       unsigned short es, unsigned int edi) {
    byte package[1];
    // 1: Check if the drive presents:
    // ==================================
    if (drive > 3 || ide_devices[drive].Reserved == 0)
        package[0] = 0x1;      // Drive Not Found!
    // 2: Check if inputs are valid:
    // ==================================
    else if (((lba + numsects) > ide_devices[drive].Size) && (ide_devices[drive].Type == IDE_ATA))
        package[0] = 0x2;                     // Seeking to invalid position.
    // 3: Read in PIO Mode through Polling & IRQs:
    // ============================================
    else {
        unsigned char err;
        if (ide_devices[drive].Type == IDE_ATA)
            err = ide_ata_access(ATA_WRITE, drive, lba, numsects, es, edi);
        else if (ide_devices[drive].Type == IDE_ATAPI)
            err = 4; // Write-Protected.
        package[0] = ide_print_error(drive, err);
    }
}
void ide_atapi_eject(unsigned char drive) {
    unsigned int   channel      = ide_devices[drive].Channel;
    unsigned int   slavebit      = ide_devices[drive].Drive;
    unsigned int   bus      = channels[channel].base;
    unsigned int   words      = 2048 / 2;               // Sector Size in Words.
    unsigned char  err = 0;
    ide_irq_invoked = 0;
    
    // 1: Check if the drive presents:
    // ==================================
    if (drive > 3 || ide_devices[drive].Reserved == 0)
        package[0] = 0x1;      // Drive Not Found!
    // 2: Check if drive isn't ATAPI:
    // ==================================
    else if (ide_devices[drive].Type == IDE_ATA)
        package[0] = 20;         // Command Aborted.
    // 3: Eject ATAPI Driver:
    // ============================================
    else {
        // Enable IRQs:
        ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN = ide_irq_invoked = 0x0);
    
        // (I): Setup SCSI Packet:
        // ------------------------------------------------------------------
        atapi_packet[ 0] = ATAPI_CMD_EJECT;
        atapi_packet[ 1] = 0x00;
        atapi_packet[ 2] = 0x00;
        atapi_packet[ 3] = 0x00;
        atapi_packet[ 4] = 0x02;
        atapi_packet[ 5] = 0x00;
        atapi_packet[ 6] = 0x00;
        atapi_packet[ 7] = 0x00;
        atapi_packet[ 8] = 0x00;
        atapi_packet[ 9] = 0x00;
        atapi_packet[10] = 0x00;
        atapi_packet[11] = 0x00;
    
        // (II): Select the Drive:
        // ------------------------------------------------------------------
        ide_write(channel, ATA_REG_HDDEVSEL, slavebit << 4);
    
        // (III): Delay 400 nanosecond for select to complete:
        // ------------------------------------------------------------------
        for(int i = 0; i < 4; i++)
            ide_read(channel, ATA_REG_ALTSTATUS); // Reading Alternate Status Port wastes 100ns.
    
        // (IV): Send the Packet Command:
        // ------------------------------------------------------------------
        ide_write(channel, ATA_REG_COMMAND, ATA_CMD_PACKET);      // Send the Command.
    
        // (V): Waiting for the driver to finish or invoke an error:
        // ------------------------------------------------------------------
        err = ide_polling(channel, 1);            // Polling and stop if error.
    
        // (VI): Sending the packet data:
        // ------------------------------------------------------------------
        asm("rep   outsw"::"c"(6), "d"(bus), "S"(atapi_packet));// Send Packet Data
        ide_wait_irq();                  // Wait for an IRQ.
        err = ide_polling(channel, 1);            // Polling and get error code.
        if (err == 3) err = 0; // DRQ is not needed here.
        package[0] = ide_print_error(drive, err); // Return;
    }
}
#endif