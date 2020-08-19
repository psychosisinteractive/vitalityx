#include "bios32.h"
#include "../libc/types.h"

bios32_sdh_t* findbios32() {
    bios32_sdh_t* sdh = (bios32_sdh_t*)0xe000;
    if(sdh->signature != BIOS32_SIGNATURE) {
        bios32_state = false;
        return sdh;
    } else {
        bios32_state = true;
        return sdh;
    }
}