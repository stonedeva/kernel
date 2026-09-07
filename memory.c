#include "./memory.h"
#include <stdint.h>

int memcmp(void* aptr, void* bptr, size_t sz)
{
    uint8_t* a = (uint8_t*)aptr;
    uint8_t* b = (uint8_t*)bptr;
    for (size_t i = 0; i < sz; i++) {
        if (a[i] < b[i]) {
            return -1;
        } else if (a[i] > b[i]) {
            return 1;
        }
    }
    return 0;
}

void memcpy(void* aptr, void* bptr, size_t sz)
{
    uint8_t* a = (uint8_t*)aptr;
    uint8_t* b = (uint8_t*)bptr;
    for (size_t i = 0; i < sz; i++) {
	b[i] = a[i];
    }
}

void memset(void* aptr, int c, size_t sz)
{
    uint8_t* a = (uint8_t*)aptr;
    for (size_t i = 0; i < sz; i++) {
	a[i] = c;
    }
}
