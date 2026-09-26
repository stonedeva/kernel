section .multiboot
align 4
    dd 0x1BADB002	; Multiboot magic
    dd 0x00000004	; Flags
    dd -(0x1BADB002 + 0x00000004)

    dd 0
    dd 0
    dd 0
    dd 0
    dd 0

    dd 0
    dd 1024
    dd 768
    dd 32

section .bss
align 16
stack_bottom:
    resb 16384	; 16 KiB stack
stack_top:

section .text
global _start
extern kmain

_start:
    mov esp, stack_top
    push ebx
    push eax
    call kmain

.hang:
    cli
    hlt
    jmp .hang
