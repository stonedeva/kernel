#include "./serial.h"
#include "./io.h"
#include <stdint.h>

#define COM1 0x3F8

void serial_init()
{
    outb(COM1 + 1, 0x00); // Disable interrupts
    outb(COM1 + 3, 0x80); // Enable DLAB
    outb(COM1 + 0, 0x03); // 38400 baud
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03); // 8 bits, no parity, one stop
    outb(COM1 + 2, 0xC7); // Enable FIFO
    outb(COM1 + 4, 0x0B);
}

void serial_putc(char c)
{
    while (!(inb(COM1 + 5) & 0x20));

    outb(COM1, c);
}

void serial_print_hex(int value)
{
    const char* hex = "0123456789ABCDEF";

    serial_putc('0');
    serial_putc('x');

    for (int i = 7; i >= 0; i--) {
        serial_putc(hex[(value >> (i * 4)) & 0xF]);
    }
    serial_putc('\n');
}

void serial_println(const char* name, int value)
{
    while (*name)
        serial_putc(*name++);

    serial_putc('=');
    serial_print_hex(value);
    serial_putc('\n');
}
