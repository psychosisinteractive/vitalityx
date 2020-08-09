[BITS 16]
    jmp start

db 'BOOTLOADER START'

load_kern_err:
    mov ah,0Ch
    mov al,0xC
    xor bh,bh
    inc cx
    int 10h
    hlt

print_dbg_x:
    mov ah,0Ch
    mov al,0xF
    xor bh,bh
    inc cx
    int 10h
    ret

start:
    mov ah,00h
    mov al,13h
    int 10h
    mov dx,0
    call print_dbg_x
    ; LOAD the system
    mov ah,02h
    mov al,11h
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
    jmp PROGRAM

PROGRAM equ 0x1000

%include "bootloader/gdt.asm"

db 'BOOTLOADER END'
times 510 - ($ - $$) db 0
dw 0AA55h