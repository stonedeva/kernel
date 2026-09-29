#include <stdint.h>
#include <stdarg.h>
#include "./kernel.h"
#include "./gdt.h"
#include "./idt.h"
#include "./io.h"
#include "./keyboard.h"
#include "./printf.h"
#include "./serial.h"
#include "./frame.h"
#include "./page.h"
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

typedef struct {
    uint8_t* addr;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint32_t bpp;
    uint8_t type;
    
    // RGB
    uint8_t red_pos;
    uint8_t red_mask;
    uint8_t green_pos;
    uint8_t green_mask;
    uint8_t blue_pos;
    uint8_t blue_mask;
} framebuffer_t;

framebuffer_t fb;


extern void isr_timer();
extern void isr_keyboard();
extern void isr_syscall();
extern void isr_page_fault();

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

void framebuffer_init(multiboot_info_t* mbi)
{
    fb.addr = (uint8_t*)(uintptr_t)mbi->framebuffer_addr;

    fb.width  = mbi->framebuffer_width;
    fb.height = mbi->framebuffer_height;
    fb.pitch  = mbi->framebuffer_pitch;
    fb.bpp    = mbi->framebuffer_bpp;
    fb.type   = mbi->framebuffer_type;

    if (fb.type == 1) {
        fb.red_pos = mbi->color_info[0];
        fb.red_mask = mbi->color_info[1];

        fb.green_pos = mbi->color_info[2];
        fb.green_mask = mbi->color_info[3];

        fb.blue_pos = mbi->color_info[4];
        fb.blue_mask = mbi->color_info[5];
    }
}

void framebuffer_dump()
{
    serial_println("fb.addr", fb.addr);
    serial_println("fb.width", fb.width);
    serial_println("fb.height", fb.height);
    serial_println("fb.pitch", fb.pitch);
    serial_println("fb.bpp", fb.bpp);
    serial_println("fb.type", fb.type);
}

void kput_pixel(int x, int y, int col)
{
    *(uint32_t*)(fb.addr + y * fb.pitch + x * 4) = col;
}

void kmain(unsigned int magic, multiboot_info_t* mbi)
{
    serial_init();
    gdt_init();
    idt_init();
//    idt_set_gate(14, (uint32_t)isr_page_fault, 0x08, 0x08E);
    idt_set_gate(32, (uint32_t)isr_timer, 0x08, 0x08E);
    idt_set_gate(33, (uint32_t)isr_keyboard, 0x08, 0x08E);
    idt_set_gate(0x80, (uint32_t)isr_syscall, 0x08, 0x0EE);
    pic_remap();
    frame_init(mbi->mem_upper + mbi->mem_lower);
    framebuffer_init(mbi);

    paging_init();

    if (!(mbi->flags & (1 << 12))) {
	return;
    }

//    framebuffer_dump();

    shell_init();

    __asm__ volatile ("sti");

    while (1) {
	__asm__ volatile("hlt");
    }
}
