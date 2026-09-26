#include "./printf.h"
#include "./font.h"
#include "./kernel.h"
#include <stdarg.h>
#include <stddef.h>


uint16_t* vga = (uint16_t*)0xb8000;
int vga_ptr = 0;


void printk_int(int n)
{
    char buf[12];
    int i = 0;

    if (n == 0) {
        //printk_ch('0');
        return;
    }

    if (n < 0) {
        //printk_ch('-');
        n = -n;
    }

    while (n > 0) {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }

    while (i > 0) {
        //printk_ch(buf[--i]);
    }
}

void printk_str(char* str)
{
    while (*str != '\0') {
        vga[vga_ptr++] = *str | (0x07 << 8);
        str++;
    }
}

void printk_ch(char c, int x, int y)
{
    uint8_t* glyph = font[(uint8_t)c - 0x20];
    /* 
     * Each font entry is converted into binary
     * 1 = pixel, 0 = empty
    */
    for (size_t row = 0; row < 8; row++) {
	for (size_t col = 0; col < 8; col++) {
	    if (glyph[row] & (1 << col)) {
		kput_pixel(x+row, y+col, 0x00FFFFFF);
	    }
	}
    }
}

/*
void printk_ch(char c)
{
    switch (c) {
    case '\n':
	vga_ptr = ((vga_ptr / 80) + 1) * 80;
	return;
    case '\b':
	if (vga_ptr < 2) return;
	vga_ptr -= 2;
	vga[vga_ptr] = ' ';

	return;
    }
    vga[vga_ptr++] = c | (0x07 << 8);
}
*/
void printk(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt != '%') {
            //printk_ch(*fmt);
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
            //printk_ch(ch);
            break;
        case 'd':
            int n = va_arg(args, int);
            printk_int(n);
            break;
        default:
            //printk_ch('%');
            //printk_ch(*fmt);
            break;
        }

        fmt++;
    }

    va_end(args);
}
