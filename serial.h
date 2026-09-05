#ifndef _SERIAL_H_
#define _SERIAL_H_

#define COM1 0x3F8

static void serial_init()
{
    outb(COM1 + 1, 0x00); // Disable interrupts
    outb(COM1 + 3, 0x80); // Enable DLAB
    outb(COM1 + 0, 0x03); // 38400 baud
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03); // 8 bits, no parity, one stop
    outb(COM1 + 2, 0xC7); // Enable FIFO
    outb(COM1 + 4, 0x0B);
}

static void serial_putc(char c)
{
    while (!(inb(COM1 + 5) & 0x20))
        ;

    outb(COM1, c);
}

static void serial_print_hex(uint32_t value)
{
    const char* hex = "0123456789ABCDEF";

    serial_putc('0');
    serial_putc('x');

    for (int i = 7; i >= 0; i--) {
        serial_putc(hex[(value >> (i * 4)) & 0xF]);
    }
}

static void serial_println(const char* name, uint32_t value)
{
    while (*name)
        serial_putc(*name++);

    serial_putc('=');
    serial_print_hex(value);
    serial_putc('\n');
}


#endif // _SERIAL_H_
