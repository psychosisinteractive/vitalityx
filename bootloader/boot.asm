[BITS 16]
[ORG 0x7c00]
    xchg bx, bx
    jmp 0:start

db 'VITALITYX BOOTSECTOR',0

%include "bootloader/gdt.asm"
[BITS 16]

start:
    ; setup video mode
    mov ah,00h
    mov al,13h
    int 10h
    mov dx,0
    call print_dbg_x
    xchg bx, bx
    ; LOAD the system
    mov ah,02h
    mov al,20
    mov ch,0
    mov cl,02h
    mov dh,0
    mov dl,0
    mov ax,0    
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov bx,PROGRAM
    int 13h
    xchg bx, bx
    call print_dbg_x
    jc load_kern_err
    ; next setup the gdt
    call print_dbg_x
    cli
    mov ax, 0x2401
    int 0x15 ; enable A20 bit
    inc cx
    call print_dbg_x
    xchg bx, bx
    call switch_to_pm

load_kern_err:
    mov ah,0Ch
    mov al,0xC
    xor bh,bh
    inc cx
    int 10h
    int 7h
    hlt
print_dbg_x:
    mov ah,0Ch
    mov al,0xF
    xor bh,bh
    inc cx
    int 10h
    ret

[BITS 32]
ready:
    mov esp, 090000h

    mov ax, DATA_SEG 
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    call PROGRAM
    jmp $

PROGRAM equ 0x1000

times 510 - ($ - $$) db 0
dw 0AA55h
