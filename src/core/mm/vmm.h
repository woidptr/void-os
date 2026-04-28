#pragma once
#include <stdint.h>
#include "pmm.h"

#define PTE_PRESENT       (1ull << 0)
#define PTE_WRITABLE      (1ull << 1)
#define PTE_USER          (1ull << 2)
#define PTE_WRITE_THROUGH (1ull << 3)
#define PTE_NO_CACHE      (1ull << 4)
#define PTE_ACCESSED      (1ull << 5)
#define PTE_DITRY         (1ull << 6)
#define PTE_HUGE_PAGE     (1ull << 7)
#define PTE_GLOBAL        (1ull << 8)
#define PTE_NO_EXECUTE    (1ull << 63)

#define pte_get_frame(entry) ((entry) & 0x000FFFFFFFFFF000)

struct boot_info;

struct page_table {
    uint64_t entries[512];
} __attribute__((aligned(PAGE_SIZE)));

struct vmm_ctx {
    uint64_t hhdm_offset;
    struct page_table* pml4;
};

void vmm_init(struct kernel_ctx* kctx, struct boot_info* info);
void vmm_map_page(struct vmm_ctx* vmm, struct pmm_ctx* kctx, uint64_t virtual_addr, uint64_t physical_addr, uint64_t flags);