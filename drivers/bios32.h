#ifndef BIOS32_H
#define BIOS32_H
#include "../libc/types.h"
#define BIOS32_SIGNATURE    0x5F32335F
#define BIOS32_REVISION     0x0

typedef struct bios32_sdh {
    // should be 0x5F32335F
    ///
    /// BIOS32 Signature 
    ///
    uint32_t signature;
    ///
    /// BIOS32 Calling Interface Entry Point
    ///
    uint32_t ciep;
    ///
    /// BIOS32 Revision, at the time of writing it is 0x0
    ///
    uint8_t revision;
    uint8_t length;
    uint8_t checksum;
    char reserved[5];
} bios32_sdh_t;

volatile bool bios32_state;

bios32_sdh_t* findbios32();

#endif