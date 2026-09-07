// Make shell a userspace program eventually

#include "./screen.h"
#include "./keyboard.h"
#include "./memory.h"

#define CMD_BUF_CAP 1024

void shell_init(uint32_t mem_kb)
{
    printk("Elox Shell v0.01 (kernel mode)\n");
    printk("Free Memory: %d KB\n", mem_kb);
    printk_ch('>');
}

char cmd[CMD_BUF_CAP];
int cmd_sz = 0; 

void shell_process_cmd()
{
    printk_ch('\n');
    printk(cmd);
}

void shell_handle_input(uint8_t scancode)
{
    if (scancode == 0x1C) {
	// Enter Key
	shell_process_cmd();
	memset(cmd, 0, cmd_sz);
	cmd_sz = 0;
	printk("\n>");
    } else {
	cmd[cmd_sz++] = keyboard_get_ch(scancode);
	printk_ch(cmd[cmd_sz-1]);
    }
}
