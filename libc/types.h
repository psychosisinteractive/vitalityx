#ifndef TYPES_H
#define TYPES_H
//byte
typedef unsigned char 	byte;
//word
typedef unsigned short 	word;
//dword
typedef unsigned int 	dword;
//boolean true/false
typedef enum {false, true} bool;
//8 bit int
typedef signed char 	int8_t;
//unsigned 8 bit int
typedef unsigned char 	uint8_t;
//16 bit int
typedef signed short int 	int16_t;
//unsigned 16 bit int
typedef unsigned short int 	uint16_t;
//32 bit int
typedef signed int 	int32_t;
//unsigned 32 bit int
typedef unsigned int 	uint32_t;
//unsigned 64 bit int
typedef unsigned long long 	uint64_t;
//64 bit int
typedef long long 	int64_t;
typedef int8_t 	    int8;
typedef uint8_t 	uint8;
typedef int16_t 	int16;
typedef uint16_t 	uint16;
typedef int32_t 	int32;
typedef uint32_t 	uint32;
typedef int64_t 	int64;
typedef uint64_t 	uint64;
typedef unsigned char 	uchar_t;
typedef uint32_t 	wchar_t;
typedef uint32_t 	size_t;
typedef uint32_t 	addr_t;
typedef int32_t 	pid_t;



// MAXIMUM INTERGER  9223372036854775807
const int intmax_t = 9223372036854775807;
// MINIMUM INTERGER  -9223372036854775807
const int intmin_t = -9223372036854775807;
// boolean


#define low_16(address) (u16)((address) & 0xFFFF)
#define high_16(address) (u16)(((address) >> 16) & 0xFFFF)

#endif