[BITS 64]

extern idt_handler
extern default_handler

section .text

%macro ISR_NOERRCODE 1
global isr%1
isr%1:
    push rbx
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rdi, %1

    call idt_handler

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    pop rbp

    iretq
%endmacro

ISR_NOERRCODE 33

global idt_load
idt_load:
    lidt [rdi]
    ret

global isr_stub
isr_stub:
    cli
    call default_handler
    hlt
