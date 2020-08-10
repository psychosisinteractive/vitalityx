[BITS 32]

global _start
_start:
    mov ax, 0x1000
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov ebp, 0x90000
    mov esp, ebp
    extern kernel
    call kernel
    ret

global idt_load
extern idtp
idt_load:
    lidt [idtp]
    ret
