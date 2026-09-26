#include "./syscall.h"

struct regs {
    int edi;
    int esi;
    int ebp;
    int esp;
    int ebx;
    int edx;
    int ecx;
    int eax;
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
