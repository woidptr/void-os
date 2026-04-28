.global gdt_load
.type gdt_load @function

gdt_load:
    lgdt (%rdi)

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    pop %rdi

    push $0x08

    push %rdi

    lretq