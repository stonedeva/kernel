#ifndef _SERIAL_H_
#define _SERIAL_H_

void serial_init();
void serial_putc(char c);
void serial_print_hex(int value);
void serial_println(const char* name, int value);

#endif // _SERIAL_H_
