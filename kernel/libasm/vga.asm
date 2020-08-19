SECTION .virt
GLOBAL sv13h
BITS 16
sv13h:
    mov ah,00h
    mov al,13h
    int 10h
    ret
GLOBAL sv3h
sv3h:
    mov ah,00h
    mov al,3h
    int 10h
    ret