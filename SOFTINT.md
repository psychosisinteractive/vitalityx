# VitalityX Interrupt 3Fh (63)

Interrupt 3Fh is the standard VitalityX software interrupt. You interrupt 3Fh to run VitalityX functions while in usermode.

For example, to write to Bochs (emulation software) output (in Intel syntax):
```
        mov eax,0x01 ; Set mode to Bochs Output
        mov ebx,string ; Set ebx to string pointer
        int 3fh ; Interrupt VitalityX
string: db 'Hello VitalityX!',0
```

Here is the current list of supported interrupts:

eax 0x01
ebx {string}
This will write {string} terminated with 0x0 to the Bochs output.

eax 0x02
ebx {string}
This will write {string} terminated with 0x0 to the graphical output.