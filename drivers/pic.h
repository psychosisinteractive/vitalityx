#ifndef PIC_H
#define PIC_H
#include "../libc/types.h"
#include "../libc/vitality/inline.h"

///
/// Master PIC
///
#define PIC1		0x20	
///
/// Slave PIC
///	
#define PIC2		0xA0		
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)
///
/// End of Interrupt
///
#define PIC_EOI		0x20	

///
/// Send End of Interrupt to PIC
///
void PIC_sendEOI(unsigned char irq);

/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */

#define ICW1_ICW4	0x01		
#define ICW1_SINGLE	0x02		
#define ICW1_INTERVAL4	0x04	
#define ICW1_LEVEL	0x08		
#define ICW1_INIT	0x10	
 
#define ICW4_8086	0x01		
#define ICW4_AUTO	0x02	
#define ICW4_BUF_SLAVE	0x08	
#define ICW4_BUF_MASTER	0x0C	
#define ICW4_SFNM	0x10	
 
///
/// Remap PIC
///@param offset1 - vector offset for master PIC	vectors on the master become offset1..offset1+7
///@param offset2 - same for slave PIC: offset2..offset2+7
///
void PIC_remap(int offset1, int offset2);

void IRQ_set_mask(unsigned char IRQline);
 
void IRQ_clear_mask(unsigned char IRQline);

#define PIC1_CMD                    0x20
#define PIC1_DATA                   0x21
#define PIC2_CMD                    0xA0
#define PIC2_DATA                   0xA1
#define PIC_READ_IRR                0x0a    /* OCW3 irq ready next CMD read */
#define PIC_READ_ISR                0x0b    /* OCW3 irq service next CMD read */
 
/* Helper func */
static uint16_t __pic_get_irq_reg(int ocw3);
/* Returns the combined value of the cascaded PICs irq request register */
uint16_t pic_get_irr(void);
/* Returns the combined value of the cascaded PICs in-service register */
uint16_t pic_get_isr(void);

#endif