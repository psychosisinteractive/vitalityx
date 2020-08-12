[BITS 32]

global _start
_start:
    jmp $
    mov ebp, 0x90000
    mov esp, ebp
    extern kernel
    call kernel
    jmp $

global idt_load
extern idtp
idt_load:
    lidt [idtp]
    ret
