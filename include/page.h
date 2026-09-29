#ifndef _PAGE_H_
#define _PAGE_H_

#include <stdint.h>

void paging_init();
void page_dir_init();
void page_table_map();
void page_fault_callback(uint32_t eip, uint32_t addr);

#endif // _PAGE_H_
