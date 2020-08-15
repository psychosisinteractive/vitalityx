# VLIB, Vitality Library

VLib is the current Vitality driver system. You manipulate it using Interrupt 3Fh (refer to VitalityX Interrupt 3Fh (63), or SOFTINT.md)

VitalityX takes care of initalizing your VLib entry for you via Int 3fh:Eax=03h, which requires the following:

- EDI - Pointer to the Function
- EDX - Mode

and it will set
    
- EBX - Entry ID

Entry ID's are how you call the driver functions. In order to call a function you must use Int 3fh:Eax=05h, which requires the following:

- EBX - Entry ID

You'd notice that Int 3fh:Eax=03h outputs to EBX, and 3fh:Eax=05h takes in EBX and that is intentional. If you want to test your VLib before running it then it would be as simple as
```
; create vlib here
mov eax,03h
int 3fh ; moves the entry id to ebx
mov eax,05h
int 3fh ; calls the entry id, which should already have been set by the previous int 3fh
```

In order to implement a VLib function, a struct is passed defining the registers where the VLib started. The struct is as defined below:

```
typedef struct {
    uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
} Registers;
```

Thus, youd define a VLib function in C as follows:
```
void vlib_function(Registers reg);
```