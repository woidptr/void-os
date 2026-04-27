#pragma once
#include <stdint.h>
#include <stddef.h>

struct framebuffer_ctx {
    uint32_t* addr;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
};

enum memmap_type {
    MEMMAP_USABLE = 0,
    MEMMAP_RESERVED = 1,
    MEMMAP_ACPI_RECLAIM = 2,
    MEMMAP_ACPI_NVS = 3,
    MEMMAP_BAD_MEMORY = 4,
    MEMMAP_BOOTLOADER = 5,
    MEMMAP_KERNEL = 6,
};

struct memmap_entry {
    uint64_t base;
    uint64_t length;
    enum memmap_type type;
};

#define MAX_MEMMAP_ENTRIES 256

struct memmap_ctx {
    uint64_t entry_count;
    struct memmap_entry entries[MAX_MEMMAP_ENTRIES];
};

struct boot_info {
    struct memmap_ctx* memory_map;
    uint64_t hhdm_offset;
    struct framebuffer_ctx* screen;
    void* rsdp;
    uint64_t kernel_physical_base;
    uint64_t kernel_virtual_base;
};