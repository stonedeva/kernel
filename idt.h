#ifndef _IDT_H_
#define _IDT_H_

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void idt_init();

#endif // _IDT_H_
