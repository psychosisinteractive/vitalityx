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
int strcmp(const char *s1, const char *s2);
int strcmpl(const char *s1, const char *s2, int len);

#endif