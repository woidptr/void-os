#include <stdint.h>
#include "core/graphics.h"
#include "core/pmm.h"
#include "core/vmm.h"
#include "core/heap.h"

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

struct kernel_ctx {
    struct framebuffer_ctx* framebuffer;
    struct pmm_ctx pmm;
    struct vmm_ctx vmm;
    struct heap_ctx heap;
};

void kernel_main(struct boot_info* info);