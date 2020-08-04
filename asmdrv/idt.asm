section .data

[extern isr_generic]
[extern isr_dividebyzero]
[extern isr_gpfault]
idt_info:
    dw idt_end - idt_start - 1
    dd idt_start
idt_start:
irq0:
	dw dividebyzero
	dw 0x0008
	db 0x00
	db 10101110b
	dw 0x0000
irq1:
	dw generic_isr
	dw 0x0008
	db 0x00
	db 10101110b
	dw 0x0000
irq2:
	dw generic_isr
	dw 0x0008
	db 0x00
	db 10101110b
	dw 0x0000
irq3:
	dw generic_isr
	dw 0x0008
	db 0x00
	db 10101110b
	dw 0x0000
irq4:
	dw generic_isr
	dw 0x0008
	db 0x00
	db 10101110b
	dw 0x0000
irq5:
	dw generic_isr
	dw 0x0008
	db 0x00
	db 10101110b
	dw 0x0000
irq6:
	dw generic_isr
	dw 0x0008
	db 0x00
	db 10101110b
	dw 0x0000
irq7:
	dw generic_isr
	dw 0x0008
	db 0x00
	db 10101110b
	dw 0x0000
irq8:	
	dw generic_isr
	dw 0x0008
	db 0x00
	db 10101110b
	dw 0x0000
irq9:
	dw generic_isr
	dw 0x0008
	db 0x00
	db 10101110b
	dw 0x0000
irqA:
	dw generic_isr
	dw 0x0008
	db 0x00
	db 10101110b
	dw 0x0000
irqB:
	dw generic_isr
	dw 0x0008
	db 0x00
	db 10101110b
	dw 0x0000
irqD:
	dw gpfault
	dw 0x0008
	db 0x00
	db 10101110b
	dw 0x0000
irqE:
	dw generic_isr
	dw 0x0008
	db 0x00
	db 10101110b
	dw 0x0000
irqF:
	dw generic_isr
	dw 0x0008
	db 0x00
	db 10101110b
	dw 0x0000
idt_end:

section .text

dividebyzero:
    pushad
    cld 
    call isr_dividebyzero
    popad
    iret

generic_isr:
	pushad
	cld
	call isr_generic
	popad
	iret

gpfault:
	pushad
	cld
    pop eax
    push eax
	call isr_gpfault
	popad
	iret