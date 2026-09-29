#include "./page.h"
#include "./serial.h"

extern void page_enable();
extern void page_load_dir(uint32_t*);

#define PAGE_SIZE 4096
#define PAGE_DIR_ENTRIES 1024

uint32_t page_dir[PAGE_DIR_ENTRIES] __attribute__((aligned(PAGE_SIZE)));
uint32_t first_page_table[PAGE_DIR_ENTRIES] __attribute__((aligned(PAGE_SIZE)));


void paging_init()
{
    page_dir_init();
    page_table_map();
    page_load_dir(page_dir);
    page_enable();
}

void page_dir_init()
{
    for (int i = 0; i < PAGE_DIR_ENTRIES; i++) {
	page_dir[i] = 0x00000002;
    }
}

void page_table_map()
{
    for (int i = 0; i < PAGE_DIR_ENTRIES; i++) {
	first_page_table[i] = (i * 0x1000) | 3;
    }
    page_dir[0] = ((uint32_t)first_page_table) | 3;
}

void page_fault_callback(uint32_t addr)
{
    serial_print_hex(addr);
}
