#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdint.h>

void shell_init(uint32_t mem_kb);
void shell_handle_input(uint8_t scancode);

#endif // _SHELL_H_
