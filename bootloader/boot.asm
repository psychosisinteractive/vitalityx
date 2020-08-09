[BITS 16]
    jmp 0:start

db 'GDT'
%include "bootloader/gdt.asm"
db 'GDT END'
db 'BOOTLOADER START'

db 'load_kern_err'
load_kern_err:
    mov ah,0Ch
    mov al,0xC
    xor bh,bh
    inc cx
    int 10h
    hlt
db 'end'
db 'print_dbg_x'
print_dbg_x:
    mov ah,0Ch
    mov al,0xF
    xor bh,bh
    inc cx
    int 10h
    ret
db 'end'
db 'start'
start:
    mov bp, 0x9000 ; set the stack
    mov sp, bp

    mov ah,00h
    mov al,13h
    int 10h
    mov dx,0
    call print_dbg_x
    ; LOAD the system
    mov ah,02h
    mov al,16h
    mov ch,0
    mov cl,02h
    mov dh,0
    mov dl,0
    mov ebx,PROGRAM
    int 13h
    ; detect errors
    ; did not load ANY sectors?
    cmp al,0
    jz load_kern_err
    mov dx,1
    call print_dbg_x
    ; next setup the gdt
    cli
    lgdt[gdt_descriptor]
    mov eax,cr0
    or eax,0x1
    mov cr0,eax
    jmp CODE_SEG:ready
db 'end'

db '32 BIT PART START HERE'

[BITS 32]
db 'ready'
ready:
    mov ax, DATA_SEG 
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    call CODE_SEG:PROGRAM
    jmp $
db 'end'

PROGRAM equ 0x1000

db 'BOOTLOADER END'
times 510 - ($ - $$) db 0
dw 0AA55h