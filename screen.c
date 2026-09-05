#include "./screen.h"
#include <stdarg.h>


uint16_t* vga = (uint16_t*)0xb8000;
uint32_t vga_ptr = 0;


void printk_int(uint32_t n)
{
    char buf[12];
    uint32_t i = 0;

    if (n == 0) {
        printk_ch('0');
        return;
    }

    if (n < 0) {
        printk_ch('-');
        n = -n;
    }

    while (n > 0) {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }

    while (i > 0) {
        printk_ch(buf[--i]);
    }
}

void printk_str(char* str)
{
    while (*str != '\0') {
        vga[vga_ptr++] = *str | (0x07 << 8);
        str++;
    }
}

void printk_ch(char c)
{
    switch (c) {
    case '\n':
	vga_ptr = ((vga_ptr / 80) + 1) * 80;
	return;
    }
    vga[vga_ptr++] = c | (0x07 << 8);
}

void printk(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt != '%') {
            printk_ch(*fmt);
            fmt++;
            continue;
        }
        fmt++; /* Skip % */

        switch (*fmt) {
        case 's':
            char* str = va_arg(args, char*);
            printk_str(str);
            break;
        case 'c':
            char ch = va_arg(args, char);
            printk_ch(ch);
            break;
        case 'd':
            uint32_t n = va_arg(args, uint32_t);
            printk_int(n);
            break;
        default:
            printk_ch('%');
            printk_ch(*fmt);
            break;
        }

        fmt++;
    }

    va_end(args);
}
