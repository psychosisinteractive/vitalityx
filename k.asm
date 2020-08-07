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

; all functions below this are for c
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

; extern void enter_v86(uint32_t ss, uint32_t esp, uint32_t cs, uint32_t eip);
enter_v86:
   mov epb, esp               ; save stack pointer

   push dword  [ebp+4]        ; ss
   push dword  [ebp+8]        ; esp
   pushfd                     ; eflags
   or dword [esp], (1 << 17)  ; set VM flags
   push dword [ebp+12]        ; cs
   push dword  [ebp+16]       ; eip
   iret

%include "asmdrv/idt.asm"
%include "drivers/vga.asm"

ORG 0x00000500

int13h:
	int 10
	int 3

section .data

idt_setup_msg:
	db "Setting up ISR...",0
idt_msg_asetup:
	db "ISR setup",0