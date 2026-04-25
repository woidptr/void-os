.code64
.extern idt_handler

.macro idtentry vector, has_error_code
    .global isr\vector
    isr\vector:
        .if \has_error_code == 0
            pushq $0
        .endif

        pushq %rbp
        pushq %rax
        pushq %rbx
        pushq %rcx
        pushq %rdx
        pushq %rsi
        pushq %rdi
        pushq %r8
        pushq %r9
        pushq %r10
        pushq %r11
        pushq %r12
        pushq %r13
        pushq %r14
        pushq %r15

        movq $\vector, %rdi
        movq %rsp, %rsi

        call idt_handler

        popq %r15
        popq %r14
        popq %r13
        popq %r12
        popq %r11
        popq %r10
        popq %r9
        popq %r8
        popq %rdi
        popq %rsi
        popq %rdx
        popq %rcx
        popq %rbx
        popq %rax
        popq %rbp

        addq $8, %rsp

        iretq
.endm

idtentry 0,  0   /* divide by zero */
idtentry 8,  1   /* double fault */
idtentry 14, 1   /* page fault */
idtentry 33, 0   /* keyboard */

.global idt_load
idt_load:
    lidt (%rdi)
    ret