/* Updated linker.ld */
OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)

PHDRS
{
    text    PT_LOAD    FLAGS((1 << 0) | (1 << 2)) ; /* Execute + Read */
    rodata  PT_LOAD    FLAGS((1 << 2)) ;            /* Read only */
    data    PT_LOAD    FLAGS((1 << 1) | (1 << 2)) ; /* Write + Read */
}

SECTIONS
{
    . = 0xffffffff80200000;

    /* Limine requests first */
    .limine_requests : {
        KEEP(*(.limine_requests))
    } :data

    /* Align to next page before code */
    . = ALIGN(CONSTANT(COMMONPAGESIZE));

    .text : {
        *(.text .text.*)
    } :text

    /* Align to next page before read-only data */
    . = ALIGN(CONSTANT(COMMONPAGESIZE));

    .rodata : {
        *(.rodata .rodata.*)
    } :rodata

    /* Align to next page before writable data */
    . = ALIGN(CONSTANT(COMMONPAGESIZE));

    .data : {
        *(.data .data.*)
    } :data

    .bss : {
        *(COMMON)
        *(.bss .bss.*)
    } :data

    /DISCARD/ : {
        *(.eh_frame)
        *(.note .note.*)
        *(.interp)
        *(.comment)
    }
}