# VLIB, Vitality Library

VLib is the current Vitality driver system. You manipulate it using Interrupt 3Fh (refer to VitalityX Interrupt 3Fh (63), or SOFTINT.md)

VitalityX takes care of initalizing your VLib entry for you via Int 3fh:Eax=03h, which requires the following:

- EDI - Pointer to the Function
- EDX - Mode

and it will set
    
- EBX - Entry ID

Entry ID's are how you call the driver functions. In order to call a function you must use Int 3fh:Eax=05h, which requires the following:

- EBX - Entry ID

You'd notice that Int 3fh:Eax=03h outputs to EBX, and 3eh takes in EBX and that is intentional. If you want to test your VLib before running it then it would be as simple as
```
; create vlib here
mov eax,03h
int 3fh ; moves the entry id to ebx
int 3eh ; calls the entry id, which should already have been set by the previous int 3fh
```

In order to implement a VLib function, a struct is passed defining the registers where the VLib started. The struct is as defined below:

```
typedef struct registers
{
   uint32_t ds;                  // Data segment selector
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   uint32_t int_no, err_code;    // Interrupt number and error code (if applicable)
   uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;
```

Thus, youd define a VLib function in C as follows:
```
void vlib_function(registers_t regs);
```

```
             +----------------------------------------------v
     +---------+                                         +-----------------+
     |  Kernel +-----------------+                       |Hardware         |
     +----^--^-+             +---v--+                    |                 |
  Int 3fh |  |               | VLib | +----------------->+-----------------+
+------+  |  |               +------+
|VLib  |--+  |
|Creator     |
|      +     |
|      |     |
+------+     |
             |
+-----+      |
|Soft |      |
|ware |      |
|with +------+
|VLIB |  Int 3eh
|perm |
+-----+
```