#include "./keyboard.h"
#include "./shell.h"
#include "./io.h"

unsigned char key_layout[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
    '9', '0', 225, '\'', '\b', /* 14: 225 ist 'ß' in CP437 */
    '\t',
    'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', 129, '+', '\n', /* 129 ist 'ü' */
    0, /* 29 - Strg (Control) */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 148, 132, '^',
    0, /* 42 - Linke Umschalttaste (Shift) */
    '#', 'y', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-',
    0, /* 54 - Rechte Umschalttaste (Shift) */
    '*',
    0, /* Alt */
    ' ', /* Leertaste */
    0, /* Caps Lock */
    0, /* 59 - F1 ... */
    0,   0,   0,   0,   0,   0,   0,   0,
    0, /* ... F10 */
    0, /* Num Lock */
    0, /* Scroll Lock */
    0, /* Home */
    0, /* Up Arrow */
    0, /* Page Up */
    '-',
    0, /* Left Arrow */
    0,
    0, /* Right Arrow */
    '+',
    0, /* End */
    0, /* Down Arrow */
    0, /* Page Down */
    0, /* Insert */
    0, /* Delete */
    0,   0, '<', /* 86 - Kleiner-als-Zeichen */
    0, /* F11 */
    0, /* F12 */
};

char keyboard_get_ch(uint8_t scancode)
{
    return key_layout[scancode];
}

void keyboard_callback()
{
    uint8_t scancode = inb(0x60);
    if (!(scancode & 0x80)) {
	//shell_handle_input(scancode);
    }
}
