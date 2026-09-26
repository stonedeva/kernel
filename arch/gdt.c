#include <stdint.h>
#include "./gdt.h"

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct gdt_entry gdt[3];
struct gdt_ptr gp;

void gdt_init()
{
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = (uint32_t)&gdt;

    gdt[0] = (struct gdt_entry){0,0,0,0,0,0};
    // Kernel Code segment 
    // (base 0, limit 4GB, access: present, ring 0, code, readable)
    gdt[1] = (struct gdt_entry){0xFFFF, 0x0000, 0x00, 0x9A, 0xCF, 0x00};
    
    // Kernel Data segment 
    // (base 0, limit 4GB, access: present, ring 0, data, writable)
    gdt[2] = (struct gdt_entry){0xFFFF, 0x0000, 0x00, 0x92, 0xCF, 0x00};

    __asm__ volatile(
        "lgdt (%0)\n\t"
        "ljmp $0x08, $flush_loc\n\t"
        "flush_loc:\n\t"
        "mov $0x10, %%ax\n\t"
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"
        "mov %%ax, %%ss\n\t"
        : : "r" (&gp) : "ax"
    );
}
