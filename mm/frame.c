#include "./frame.h"
#include "./memory.h"
#include "./kernel.h"
#include <stdint.h>

#define FRAME_SIZE 4096
#define MAX_FRAMES 32768
#define BITMAP_CAP ((MAX_FRAMES + 7) / 8)

extern char kernel_end[];
void* frame_start;
size_t total_frames = 0;

uint8_t bitmap[BITMAP_CAP];
size_t bitmap_sz;


void frame_init(size_t avail_mem)
{
    uintptr_t start = (uintptr_t)&kernel_end;
    start = (start + FRAME_SIZE - 1) & ~(FRAME_SIZE - 1); // align to next 4KiB
    frame_start = (void*)start;
    total_frames = avail_mem / FRAME_SIZE;
    bitmap_sz = (total_frames + 7) / 8;
    memset(bitmap, 0, bitmap_sz);
}

void frame_bm_set(size_t n)
{
    uint8_t byte = n / 8;
    uint8_t bit = n % 8;
    bitmap[byte] |= (1u << bit);
}

void frame_bm_clear(size_t n)
{
    size_t byte = n / 8;
    size_t bit = n % 8;
    bitmap[byte] &= ~(1u << bit);
}

int frame_bm_is_used(size_t n)
{
    size_t byte = n / 8;
    size_t bit = n % 8;
    return (bitmap[byte] & (1u << bit));
}

void* frame_alloc()
{
    for (size_t i = 0; i < bitmap_sz; i++) {
	if (!frame_bm_is_used(i)) {
	    frame_bm_set(i);
	    void* frame_ptr = (char*)frame_start + FRAME_SIZE*i;
	    return frame_ptr;
	}
    }

    return 0;
}

void frame_free(void* frame_ptr)
{
    uintptr_t addr = (uintptr_t)frame_ptr;
    uintptr_t start = (uintptr_t)frame_start;
    if (addr < start) {
	return;
    }

    size_t offset = addr - start;
    if (offset % FRAME_SIZE != 0) {
	return;
    }
    size_t bm_index = offset / FRAME_SIZE;
    if (bm_index >= total_frames) {
	return;
    }

    if (frame_bm_is_used(bm_index)) {
	frame_bm_clear(bm_index);
	frame_ptr = 0;
    }
}
