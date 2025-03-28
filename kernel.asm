bits 32

section .text
    align 4
    dd 0x1BADB002           ; magic
    dd 0x00                 ; flags
    dd -(0x1BADB002 + 0x00) ; checksum

global start
global keyboard_handler
global read_port
global write_port
global load_idt

global outb
extern kmain
extern keyboard_handler_main

outb:
    mov dx, [esp + 4]  ; Второй аргумент (порт)
    mov al, [esp + 8]  ; Первый аргумент (значение)
    out dx, al
    ret

read_port:
    mov edx, [esp + 4]
    in al, dx   
    ret

write_port:
    mov   edx, [esp + 4]    
    mov   al, [esp + 4 + 4]  
    out   dx, al  
    ret

load_idt:
    mov edx, [esp + 4]
    lidt [edx]
    sti
    ret

keyboard_handler:
    call keyboard_handler_main
    iretd

start:
    cli
    mov esp, stack_space + 8192
    call kmain
    hlt

section .bss
resb 8192
stack_space:

section .note.GNU-stack
