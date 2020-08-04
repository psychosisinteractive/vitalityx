; k.asm

global _start
global setup_isr

section .text

[bits 32]
extern tty_putstr
_start:  
	extern kernel
	call kernel
	hlt

setup_isr:
	push 4
	push 0
	push idt_setup_msg
	call tty_putstr
	lidt[idt_info]
	push 4
	push 0
	push idt_msg_asetup
	call tty_putstr
	ret

%include "asmdrv/idt.asm"

section .data


idt_setup_msg:
	db "Setting up ISR...",0
idt_msg_asetup:
	db "ISR setup",0