///
/// This file is licensed under a seperated license listed below.
/// BSD 2-Clause License
///
/// Copyright (c) 2020-2021, Psychosis Interactive All rights reserved.
/// 
/// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
/// 
/// Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
/// Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/// 
#include "mem.h"
#include "../types.h"
#include "../system.h"
#include "../ext/debug.h"
#include "inline.h"
#include "../string.h"

#define MEMENTRY_SIZE 255

mementry_t mementries[MEMENTRY_SIZE];

void setupmalloc() {
    BochsConsolePrintString("\nSetting up memory allocator\n");
    memset(&mementries,0,sizeof(mementry_t)*MEMENTRY_SIZE);
    malloc(0x01000000,255,"vitality-system");
    malloc(0x000A0000,64000,"graphic-display");
    BochsConsolePrintString("\nMementries located at ");
    char mementry[50];
    memset(&mementry,0,50);
    itoa(&mementries,mementry);
    BochsConsolePrintString(mementry);
    BochsConsolePrintString("\n");
}

uint32_t malloc(uint32_t base, uint32_t size, char *purpose) {
    BochsConsolePrintString("\nNew memory allocation being made\n");
    BochsConsolePrintString("Purpose: ");
    BochsConsolePrintString(purpose);
    BochsConsolePrintString("\n");
    int i = 0;
    bool mallocwork = false;
    for(i = 0; i < MEMENTRY_SIZE; i++) {
        if(!mementries[i].instantiated) {
            mallocwork = true;
            break;
        } else {
            continue;
        }
    }
    if(!mallocwork) {
        return 0; // nothing
    }
    mementry_t entry;

    memcpy(&entry.purpose,purpose,16);
    memset(base,0x00,size);
    entry.instantiated = true;
    entry.base = base;
    entry.size = size;
    mementries[i] = entry;
    return i;
}

void memfree(uint32_t memid) {
    
}

addr_t malloctoaddr(uint32_t memid, uint32_t addr) {
    addr += mementries[memid].base;
    return (addr_t)addr;
}

void mempoke(uint32_t memid, uint32_t addr, uint8_t v) {
    addr_t maddr = malloctoaddr(memid,addr);
    *maddr = v;
}

uint8_t mempeek(uint32_t memid, uint32_t addr) {
    addr_t maddr = malloctoaddr(memid,addr);
    return *maddr;
}