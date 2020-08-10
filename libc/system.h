#ifndef SYSTEM_H
#define SYSTEM_H
#include "types.h"

///
/// Poke Byte
///@param addr The address of the byte
///@param offset The offset from the address
///@param val The value 
///
void pokeb(uint32_t *addr, uint32_t offset, uint8_t val);
///
/// Peek Byte
///@param addr The address of the byte
///@param offset The offset from the address
///
uint8_t peekb(uint8_t *addr, uint32_t offset);
///
/// Memset
///@param dest The destination
///@param val The value to set
///@param len The length of bytes to set
///
void *memset(addr_t dest, int val, size_t len);
#endif