///
/// This flag means the source file is being compiled under C++ mode
///
#define CPP

#include "../vitality/tty.h"
#include "../types.h"
#include "cpp_classdef.h"

///
/// C++ Pure Virtual Function, called when a virtual function is required YET there is nothing to handle its call
///
extern "C" void __cxa_pure_virtual()
{
    tty_pputstring(c_stc("[KERNEL ERROR]__cxa_pure_virtual called, No class spec, halting"));
    
}

///
/// Converts a const char array into a char*
///@param string The array
///
char* c_stc(const char string[]) {
    return (char*)&string[0];
}