OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(kernel_main)

PHDRS
{
    text    PT_LOAD    FLAGS((1 << 0) | (1 << 2)) ; /* Execute + Read */
    rodata  PT_LOAD    FLAGS((1 << 2)) ;            /* Read only */
    data    PT_LOAD    FLAGS((1 << 1) | (1 << 2)) ; /* Write + Read */
}

SECTIONS
{
    /* Limine will load us at this higher-half memory address */
    . = 0xffffffff80200000;

    /* 1. CODE (Execute + Read) */
    .text : {
        *(.text .text.*)
    } :text

    /* Move to a fresh 4KB page */
    . = ALIGN(4K);

    /* 2. READ-ONLY DATA */
    .rodata : {
        *(.rodata .rodata.*)
    } :rodata

    /* Move to a fresh 4KB page */
    . = ALIGN(4K);

    /* 3. WRITABLE DATA (Group all your data here!) */
    .data : {
        /* Put the requests safely inside the contiguous data segment */
        KEEP(*(.limine_requests))
        *(.data .data.*)
    } :data

    /* 4. UNINITIALIZED DATA (Also belongs to the data segment) */
    .bss : {
        *(COMMON)
        *(.bss .bss.*)
    } :data

    /* 5. JUNK WE DON'T NEED */
    /DISCARD/ : {
        *(.eh_frame)
        *(.note .note.*)
        *(.interp)
        *(.comment)
    }
}