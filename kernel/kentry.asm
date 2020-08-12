[BITS 32]

global _start
_start:
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

extern loadPageDirectory
loadPageDirectory:
    push ebp
    mov esp, ebp
    mov [esp], eax
    mov eax, cr3
    mov ebp, esp
    pop ebp
    ret

extern enablePaging
enablePaging:
    push ebp
    mov esp, ebp
    mov cr0, eax
    or eax,0x80000000
    mov eax, cr0
    mov ebp, esp
    pop ebp
    ret
