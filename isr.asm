global isr_timer
global isr_keyboard
extern timer_callback
extern keyboard_callback

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
