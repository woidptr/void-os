/* Multiboot Header Constants */
.set ALIGN,    1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,  1<<1             /* provide memory map */
.set FLAGS,    ALIGN | MEMINFO  /* this is the Multiboot 'flag' field */
.set MAGIC,    0x1BADB002       /* 'magic number' lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */

/* Declare the Multiboot Header */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* Reserve a stack for the initial C code (16KB) */
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

/* The entry point of our OS */
.section .text
.global _start
.type _start, @function
_start:
    /* Set up the stack pointer */
    mov $stack_top, %esp

    /* Call the C kernel */
    call kmain

    /* If the kernel returns, disable interrupts and halt the CPU infinitely */
    cli
1:  hlt
    jmp 1b