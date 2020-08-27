[BITS 16]
[ORG 0x7c00]
    jmp 0:start

%include "bootloader/gdt.asm"
[BITS 16]

start:
    call print_dbg_x
    ; LOAD the system
    mov ah,02h
    mov al, PROGRAM_SIZE / 512
    mov ch,0
    mov cl,02h
    mov dh,0
    mov bx, PROGRAM
    int 13h
    
    ;call print_dbg_x
    jc load_kern_err
    ; next setup the gdt
    call print_dbg_x
    mov di,0x7e00

    cli
    mov ax, 0x2401
    int 0x15 ; enable A20 bit

    call switch_to_pm

load_kern_err:
    ;impl later
    hlt
print_dbg_x:
    ;impl later
    ret
ns:
    hlt

[BITS 32]
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

    jmp PROGRAM

PROGRAM equ 0x7e00
PROGRAM_SIZE equ 32768  ; in bytes
                 
times 510 - ($ - $$) db 0
dw 0AA55h