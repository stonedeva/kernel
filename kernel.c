#include <stdint.h>
#include <stdarg.h>
#include "./gdt.h"
#include "./idt.h"
#include "./io.h"
#include "./keyboard.h"
#include "./screen.h"
#include "./shell.h"

/*
 * Multiboot info provided by GRUB bootloader
*/
typedef struct {
    uint32_t flags;

    uint32_t mem_lower;
    uint32_t mem_upper;

    uint32_t boot_device;
    uint32_t cmdline;

    uint32_t mods_count;
    uint32_t mods_addr;

    uint32_t syms[4];

    uint32_t mmap_length;
    uint32_t mmap_addr;

    uint32_t drives_length;
    uint32_t drives_addr;

    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;

    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;

    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    uint8_t framebuffer_type;

    uint8_t color_info[6];
} __attribute__((packed)) multiboot_info_t;


extern void isr_timer();
extern void isr_keyboard();
extern void isr_syscall();

volatile uint32_t timer = 0;


void pic_remap()
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // ICW2 - Map Master PIC to 0x20 (32) 
    // and Slave PIC to 0x28 (40)
    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    // ICW3 - Tell Master PIC that 
    // there is a slave PIC at IRQ2 (0x04)
    outb(0x21, 0x04);
    // Tell Slave PIC its cascade identity (0x02)
    outb(0xA1, 0x02);

    // ICW4 - Set 8086 mode
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
}

void timer_callback()
{
    timer++;
}

void kmain(unsigned int magic, multiboot_info_t* mbi)
{
    gdt_init();
    idt_init();
    idt_set_gate(32, (uint32_t)isr_timer, 0x08, 0x08E);
    idt_set_gate(33, (uint32_t)isr_keyboard, 0x08, 0x08E);
    idt_set_gate(0x80, (uint32_t)isr_syscall, 0x08, 0x0EE);
    pic_remap();
    
    shell_init(mbi->mem_lower + mbi->mem_upper);

    __asm__ volatile ("sti");

    while (1) {
	__asm__ volatile("hlt");
    }
}
