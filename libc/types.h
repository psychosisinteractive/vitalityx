#ifndef TYPES_H
#define TYPES_H

// 64 BIT UNSIGNED INTERGER
typedef unsigned long   uint64_t;
// 64 BIT SIGNED INTERGER
typedef          long   sint64_t;
// 32 BIT UNSIGNED INTERGER
typedef unsigned int   uint32_t;
// 32 BIT SIGNED INTERGER
typedef          int   sint32_t;
// 16 BIT UNSIGNED INTERGER
typedef unsigned short uint16_t;
// 16 BIT SIGNED INTERGER
typedef          short sint16_t;
// 8 BIT UNSIGNED INTERGER
typedef unsigned char  uint8_t;
// 8 BIT SIGNED INTERGER
typedef          char  sint8_t;

// MAXIMUM INTERGER  9223372036854775807
const int intmax_t = 9223372036854775807;
// MINIMUM INTERGER  -9223372036854775807
const int intmin_t = -9223372036854775807;
// boolean
typedef enum {false, true} bool;


#define low_16(address) (u16)((address) & 0xFFFF)
#define high_16(address) (u16)(((address) >> 16) & 0xFFFF)

#endif