#ifndef _KERNEL_H_
#define _KERNEL_H_

void printk_int(int n);
void printk_str(char* str);
void printk_ch(char c, int x, int y);
void printk(const char* fmt, ...);

#endif // _KERNEL_H_
