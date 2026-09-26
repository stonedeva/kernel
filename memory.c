#include "./memory.h"
#include <stdint.h>

extern char kernel_end[];
uint8_t* heap_start = (uint8_t*)&kernel_end;

void* memset(void* ptr, int c, size_t n)
{
    uint8_t* p = (uint8_t*)ptr;
    for (size_t i = 0; i < n; i++) {
	p[i] = (uint8_t)c;
    }
    return ptr;
}
