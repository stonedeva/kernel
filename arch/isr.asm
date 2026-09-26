global isr_timer
global isr_keyboard
global isr_syscall
extern timer_callback
extern keyboard_callback
extern syscall_callback

isr_timer:
    pushad
    call timer_callback
    mov al, 0x20
    out 0x20, al
    popad
    iret

isr_keyboard:
    pushad
    call keyboard_callback
    mov al, 0x20
    out 0x20, al
    popad
    iret

isr_syscall:
    pushad
    call syscall_callback
    popad
    iret
