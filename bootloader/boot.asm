[BITS 16]
[ORG 0x7c00]
    jmp 0:start

%include "bootloader/gdt.asm"
[BITS 16]

start:
    call reset_floppy
    call print_dbg_x
    ; LOAD the system
    ; only loads 64 sectors
    ; system will load the rest of its data
    mov ax, BLOCK64SIZE
    mov bx, PROGRAM
    mov al,BLOCKSESIZE
    call read_floppy
    
    ;mov ax, 0x800
    ;mov es,ax
    ;mov bx, PROGRAM
    ;mov al,BLOCKSESIZE
    ;call read_floppy
    
    ;call print_dbg_x
    ; next setup the gdt
    call print_dbg_x
    mov di,0x7e00

    cli
    mov ax, 0x2401
    int 0x15 ; enable A20 bit

    call switch_to_pm

load_kern_err:
    ;impl later
    jmp $
print_dbg_x:
    ;impl later
    ret
ns:
    jmp $
read_floppy:
    pusha
    mov ah,02h
    mov ch,0
    mov cl,02h
    mov dh,0
    int 13h
    jc load_kern_err
    call reset_floppy
    popa
    ret
reset_floppy:
    pusha
    mov ah,00h 
    int 13h
    popa
    ret

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
SECTOR_SIZE equ 512
BLOCKSESIZE equ 63
BLOCK64SIZE equ SECTOR_SIZE*BLOCKSESIZE
                 
times 510 - ($ - $$) db 0
dw 0AA55h