global isr_timer
global isr_keyboard
global isr_syscall
global isr_page_fault
extern timer_callback
extern keyboard_callback
extern syscall_callback
extern page_fault_callback

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

isr_page_fault:
    cli
    pushad

;    mov eax, cr2
;    push eax
;    call page_fault_callback
;    add esp, 4
;    popad
;    add esp, 4
;    iret
    mov eax, cr2
    mov edx, [esp + 36]
    push eax
    push edx
    call page_fault_callback
    add esp, 8
    popad
    add esp, 4
    iretd
