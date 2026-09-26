#include "./printf.h"
#include "./font.h"
#include "./kernel.h"
#include <stdarg.h>
#include <stddef.h>

int x_cursor = 0;
int y_cursor = 0;
int fscale = 2;


void printk_int(int n)
{
    char buf[12];
    int i = 0;

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
	printk_ch(*str);
        str++;
    }
}

void printk_ch(char c)
{
    uint8_t* glyph = font[(uint8_t)c - 0x20];
    /* 
     * Each font entry is converted into binary
     * 1 = pixel, 0 = empty
    */
    for (size_t row = 0; row < 8; row++) {
	for (size_t col = 0; col < 8; col++) {
	    if (glyph[row] & (0x80 >> col)) {
		for (size_t dy = 0; dy < fscale; dy++) {
		    for (size_t dx = 0; dx < fscale; dx++) {
			kput_pixel(x_cursor + col * fscale + dx, 
				   y_cursor + row * fscale + dy, 
				   0x00FFFFFF);
		    }
		}
	    }
	}
    }
    x_cursor += 8*fscale;
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
            int n = va_arg(args, int);
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
