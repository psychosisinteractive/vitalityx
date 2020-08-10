[BITS 16]
[ORG 0x7c00]
    jmp 0:start

%include "bootloader/gdt.asm"
[BITS 16]

start:
    ; setup video mode
    mov ah,00h
    mov al,13h
    int 10h
    mov dx,0
    call print_dbg_x
    ; LOAD the system
    mov ah,02h
    mov al,20
    mov ch,0
    mov cl,02h
    mov dh,0
    mov dl,0
    mov ebx,PROGRAM
    int 13h
    call print_dbg_x
    ; next setup the gdt
    call print_dbg_x
    cli
    mov ax, 0x2401
    int 0x15 ; enable A20 bit

    call switch_to_pm

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

db '32 BIT PART START HERE'

[BITS 32]
db 'ready'
ready:
    mov eax,0xff
    mov [0xa000],eax

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
db 'end'

PROGRAM equ 0x1000

db 'BOOTLOADER END'
times 510 - ($ - $$) db 0
dw 0AA55h
