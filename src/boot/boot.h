#pragma once
#include <stdint.h>
#include <stddef.h>

/**
 * @file boot.h
 * 
 * @brief The Unified Bootloader Interface
 * 
 * This file defines the strict, architecture-agnostic contract between the 
 * physical bootloader (e.g., Limine, Multiboot2, UEFI) and the Operating System core.
 * 
 * @note The initial architecture entry point (e.g., `_start`) MUST parse the 
 * proprietary bootloader structures, fully populate the `boot_info` structure 
 * defined here, and ONLY then jump to `kernel_main()`. The core kernel logic 
 * relies on this data being 100% accurate and sanitized.
 */

struct framebuffer {
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

/**
 * @brief The Master Boot Information structure.
 * * This is the sole parameter passed into `kernel_main()`. It contains every 
 * piece of hardware information the kernel needs to bootstrap its memory managers, 
 * hardware discovery, and display systems.
 * 
 * @warning Do NOT invoke `kernel_main()` until every field in this structure 
 * has been successfully verified and populated by the boot shim.
 */
struct boot_info {
    struct memmap_ctx* memory_map;
    uint64_t hhdm_offset;
    struct framebuffer* screen;
    void* rsdp;
    uint64_t kernel_physical_base;
    uint64_t kernel_virtual_base;
};