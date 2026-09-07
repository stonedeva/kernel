#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>

char keyboard_get_ch(uint8_t scancode);
void keyboard_callback();

#endif // _KEYBOARD_H_
