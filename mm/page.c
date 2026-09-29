#include "./page.h"
#include "./serial.h"

extern void page_enable();
extern void page_load_dir(uint32_t*);

#define PAGE_SIZE 4096
#define PAGE_DIR_ENTRIES 1024

uint32_t page_dir[PAGE_DIR_ENTRIES] 
    __attribute__((aligned(PAGE_SIZE)));
uint32_t first_page_table[PAGE_DIR_ENTRIES] 
    __attribute__((aligned(PAGE_SIZE)));
uint32_t fb_page_table[PAGE_DIR_ENTRIES] 
    __attribute__((aligned(PAGE_SIZE)));


void paging_init(uint32_t fb_phys_addr, uint32_t fb_sz)
{
    page_dir_init();
    page_table_map();
    framebuffer_map(fb_phys_addr, fb_sz);
    page_load_dir(page_dir);
    page_enable();
}

void page_dir_init()
{
    for (uint32_t i = 0; i < PAGE_DIR_ENTRIES; i++) {
	page_dir[i] = 0x00000002;
    }
}

void page_table_map()
{
    for (uint32_t i = 0; i < PAGE_DIR_ENTRIES; i++) {
	first_page_table[i] = (i * 0x1000) | 3;
    }
    page_dir[0] = ((uint32_t)first_page_table) | 3;
}

void framebuffer_map(uint32_t phys_addr, uint32_t sz)
{
    uint32_t fb_start = phys_addr & ~0xFFF;
    uint32_t fb_end = (phys_addr + sz + 0xFFF) & ~0xFFF;

    uint32_t pages = (fb_end - fb_start) / PAGE_SIZE;

    for (uint32_t i = 0; i < pages; i++) {
        fb_page_table[i] = (fb_start + i * PAGE_SIZE) | 3;
    }

    // This example maps the framebuffer virtually at the
    // 4 MiB virtual address.
    page_dir[1] = ((uint32_t)fb_page_table) | 3;
}

void page_fault_callback(uint32_t eip, uint32_t addr)
{
    serial_println("Addr", addr);
    serial_println("EIP", eip);
    
    while (1) {
	asm volatile("hlt");
    }
}
