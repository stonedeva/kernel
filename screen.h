#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <stdint.h>

void printk_int(uint32_t n);
void printk_str(char* str);
void printk_ch(char c);
void printk(const char* fmt, ...);

#endif // _KERNEL_H_
