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
#ifndef MEM_H
#define MEM_H
#include "../types.h"

typedef struct mementry {
    bool instantiated;
    uint32_t base;
    uint32_t size;
    char purpose[16];
} mementry_t;

void setupmalloc();
uint32_t malloc(uint32_t base, uint32_t size, char *purpose);
void memfree(uint32_t memid);

void mempoke(uint32_t memid, uint32_t addr, uint8_t v);
uint8_t mempeek(uint32_t memid, uint32_t addr);
addr_t malloctoaddr(uint32_t memid, uint32_t addr);

#endif