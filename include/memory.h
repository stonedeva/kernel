#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stddef.h>

void* memset(void* ptr, int c, size_t n);
int strcmp(char* s1, char* s2);
void memcpy(void* dest, const void* src, size_t n);

#endif // _MEMORY_H_
