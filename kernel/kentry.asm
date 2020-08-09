[BITS 32]

PM_STACK equ 0x001F0000

global _start
_start:
    mov ax, 0x1000
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, PM_STACK
    extern kernel
    call kernel
    jmp $