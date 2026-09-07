#include "./syscall.h"
#include <stdint.h>

struct regs {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
};

void syscall_callback(struct regs* r)
{
    switch (r->eax) {
    case SYS_WRITE:
	break;
    case SYS_READ:
	break;
    }
}
