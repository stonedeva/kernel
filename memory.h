#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stddef.h>

int memcmp(void* aptr, void* bptr, size_t sz);
void memcpy(void* aptr, void* bptr, size_t sz);
void memset(void* aptr, int c, size_t sz);

#endif // _MEMORY_H_
