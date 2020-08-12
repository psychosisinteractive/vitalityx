gdt_start: ; don't remove the labels, they're needed to compute sizes and jumps
gdt_null:
    dq 0


; GDT for code segment. base = 0x00000000, length = 0xfffff
; for flags, refer to os-dev.pdf document, page 36
gdt_code: 
    dw 0xffff    ; segment length, bits 0-15
    dw 0x0       ; segment base, bits 0-15
    db 0x0       ; segment base, bits 16-23
    db 10011010b ; flags (8 bits)
    db 11001111b ; flags (4 bits) + segment length, bits 16-19
    db 0x0       ; segment base, bits 24-31

; GDT for data segment. base and length identical to code segment
; some flags changed, again, refer to os-dev.pdf
gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_end:

; GDT descriptor
db "GDT DESCRIPTOR"
gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; size (16 bit), always one less of its true size
    dd gdt_start ; address (32 bit)
db "END DESCRIPTOR"

; define some constants for later use
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

[bits 16]
switch_to_pm:
    cli     
    mov ax,0xa00f
    mov ds,ax
    mov byte[0x000],0xf

    mov ax, DATA_SEG ; 5. update the segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    xchg bx, bx
    lgdt [gdt_descriptor]  ; THIS is where the general protection fault occours 
    mov eax, cr0
    or eax, 0x1 ; 3. set 32-bit mode bit in cr0
    mov cr0, eax    
    xchg bx, bx
    jmp init_pm ; 4. far jump by using a different segment6

[bits 32]
init_pm: ; we are now using 32-bit instructions
    xchg bx, bx
    mov ax, DATA_SEG ; 5. update the segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov byte [0xa0000], 0xf

    mov ebp, 0x90000 ; 6. update the stack right at the top of the free space
    mov esp, ebp
    xchg bx, bx
    call ready ; 7. Call a well-known label with useful code
