#ifndef TYPES_H
#define TYPES_H

///
/// Unsigned 8-bit Integer
///
typedef unsigned char uint8_t;
///
/// Signed 8-bit Integer
///
typedef char sint8_t;
///
/// Unsigned 32-bit Integer
///
typedef unsigned short uint16_t;
///
/// Signed 16-bit Integer
///
typedef short sint16_t;
///
/// Unsigned 32-bit Integer
///
typedef unsigned int uint32_t;
///
/// Signed 32-bit Integer
///
typedef int sint32_t;
///
/// Unsigned 64-bit Integer
///
typedef unsigned long uint64_t;
///
/// Signed 64-bit Integer
///
typedef long sint64_t;
#ifndef CPP
///
/// Boolean Value, true or false
///
typedef enum { false, true } bool;
#endif
///
/// Address Pointer
///
typedef uint32_t *addr_t;
///
/// Size Container
///
typedef uint32_t size_t;

///
/// Gets the High 16 bits of a number
///
#define low_16(address) (uint16_t)((address) & 0xFFFF)
///
/// Gets the Low 16 bits of a number
///
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

///
/// Null value
///
#define NULL 0

///
/// Maximum UShort16 Value
///
#define USHRT_MAX 65535 

///
/// Rename of uint8_t to byte
///
typedef uint8_t byte;



///
/// Struct for Registers
///
typedef struct {
    uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
} Registers;

///
/// Struct for Interrupt Registers
///
typedef struct registers
{
   uint32_t ds;                  // Data segment selector
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   uint32_t int_no, err_code;    // Interrupt number and error code (if applicable)
   uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;

///
/// Function Pointer
///
typedef void(*func_ptr)(void);
///
/// Function Pointer with Register Arguments
///
typedef int(*func_ptr_args)(registers_t);

///
/// ALIGN to 16 bytes
///
typedef char U_ALIGN16[16];

typedef sint8_t s8int;
typedef uint8_t u8int;
typedef sint16_t s16int;
typedef uint16_t u16int;
typedef sint32_t s32int;
typedef uint32_t u32int;
typedef sint64_t s64int;
typedef uint64_t u64int;

#endif