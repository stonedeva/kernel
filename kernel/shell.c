// Make shell a userspace program eventually

#include "./printf.h"
#include "./kernel.h"
#include "./keyboard.h"
#include "./memory.h"

#define CMD_BUF_CAP 1024

void shell_init()
{
    printk("Elox Shell v0.01 (kernel mode)\n");
    printk_ch('>');
}

char cmd[CMD_BUF_CAP];
int cmd_sz = 0; 

void shell_process_cmd()
{
    printk_ch('\n');
    if (strcmp(cmd, "version") == 0) {
	printk("v0.01a");
    }
}

void shell_handle_input(uint8_t scancode)
{
    switch (scancode) {
    case 0x1C:
	// Enter Key
	shell_process_cmd();
	memset(cmd, 0, cmd_sz);
	cmd_sz = 0;
	printk("\n>");
	break;
    case 0x0E:
	// Backspace
	if (cmd_sz > 0) {
	    cmd_sz--;
	    cmd[cmd_sz] = '\0';
	    printk_ch('\b');
	}
	break;
    default:
	cmd[cmd_sz++] = keyboard_get_ch(scancode);
	printk_ch(cmd[cmd_sz-1]);
	break;
    }
}
