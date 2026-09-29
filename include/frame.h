#ifndef _FRAME_H_
#define _FRAME_H_

#include <stddef.h>

void frame_init(size_t avail_mem);
void* frame_alloc();
void frame_free(void* frame_ptr);

#endif // _FRAME_H_
