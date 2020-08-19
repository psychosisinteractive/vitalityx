GLOBAL enter_v86
enter_v86:
    mov [0x1000], esp               ; save stack pointer

    push dword  [ebp+4]        ; ss
    push dword  [ebp+8]        ; esp
    pushfd                     ; eflags
    or dword [esp], (1 << 17)  ; set VM flags
    push dword [ebp+12]        ; cs
    push dword  [ebp+16]       ; eip
    mov esp, [0x1000]
    iret
GLOBAL detect_v86
detect_v86:
    smsw    ax
    and     eax,1           ;CR0.PE bit
    ret
