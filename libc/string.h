#ifndef STRING_H
#define STIRNG_H
#include "types.h"

///
/// String typedef
///
typedef char* string_t;

size_t strlen(const char * str);
void reverse(char s[]);
char *itoa( int value, char * str, int base);
int atoi(char* str);

#endif