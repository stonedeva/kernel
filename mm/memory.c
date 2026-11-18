#include "./memory.h"
#include <stdint.h>

void* memset(void* ptr, int c, size_t n)
{
    uint8_t* p = (uint8_t*)ptr;
    for (size_t i = 0; i < n; i++) {
	p[i] = (uint8_t)c;
    }
    return ptr;
}

int strcmp(char* s1, char* s2)
{
    while (*s1 && (*s1 == *s2)) {
	s1++;
	s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

void memcpy(void* dest, const void* src, size_t n)
{
    uint8_t* dest8 = (uint8_t*)dest;
    const uint8_t* src8 = (const uint8_t*)src;
    for (size_t i = 0; i < n; i++) {
	dest8[i] = src8[i];
    }
}
