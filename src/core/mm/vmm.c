#include "vmm.h"
#include "kernel.h"
#include "lib/cmem.h"

void vmm_init(struct kernel_ctx* kctx, struct boot_info* info) {
    void* pml4_phys = pmm_alloc_page(&kctx->pmm);
    if (pml4_phys == nullptr) {
        for (;;) __asm__("hlt");
    }

    kctx->vmm.hhdm_offset = info->hhdm_offset;

    kctx->vmm.pml4 = (struct page_table*)((uint64_t)pml4_phys + kctx->vmm.hhdm_offset);
    memset(kctx->vmm.pml4, 0, PAGE_SIZE);

    for (size_t i = 0; i < info->memory_map->entry_count; i++) {
        struct memmap_entry* entry = &info->memory_map->entries[i];

        uint64_t base = entry->base & ~(PAGE_SIZE - 1);
        uint64_t top = (entry->base + entry->length + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);

        bool is_kernel = (entry->type == MEMMAP_KERNEL);

        for (uint64_t phys = base; phys < top; phys += PAGE_SIZE) {
            uint64_t hhdm_virt = phys + kctx->vmm.hhdm_offset;
            vmm_map_page(&kctx->vmm, &kctx->pmm, hhdm_virt, phys, PTE_PRESENT | PTE_WRITABLE);

            if (is_kernel && phys >= info->kernel_physical_base) {
                uint64_t kernel_virt = info->kernel_virtual_base + (phys - info->kernel_physical_base);
                vmm_map_page(&kctx->vmm, &kctx->pmm, kernel_virt, phys, PTE_PRESENT | PTE_WRITABLE);
            }
        }
    }

    __asm__ volatile (
        "mov %0, %%cr3"
        :
        : "r" ((uint64_t)pml4_phys)
        : "memory"
    );
}

static struct page_table* get_next_level(struct page_table* parent_table, struct vmm_ctx* vmm, struct pmm_ctx* pmm, size_t index, uint64_t flags) {
    uint64_t entry = parent_table->entries[index];

    if ((entry & PTE_PRESENT) != 0) {
        uint64_t phys_addr = pte_get_frame(entry);

        return (struct page_table*)(phys_addr + vmm->hhdm_offset);
    }

    void* new_page = pmm_alloc_page(pmm);
    if (new_page == nullptr) {
        for (;;) __asm__("hlt");
    }

    struct page_table* new_table = (struct page_table*)((uint64_t)new_page + vmm->hhdm_offset);
    memset(new_table, 0, PAGE_SIZE);

    parent_table->entries[index] = (uint64_t)new_page | flags | PTE_PRESENT | PTE_WRITABLE;

    return new_table;
}

void vmm_map_page(struct vmm_ctx* vmm, struct pmm_ctx* pmm, uint64_t virtual_addr, uint64_t physical_addr, uint64_t flags) {
    virtual_addr &= ~(PAGE_SIZE - 1);
    physical_addr &= ~(PAGE_SIZE - 1);

    size_t pml4_idx = (virtual_addr >> 39) & 0x1FF;
    size_t pdpt_idx = (virtual_addr >> 30) & 0x1FF;
    size_t pd_idx   = (virtual_addr >> 21) & 0x1FF;
    size_t pt_idx   = (virtual_addr >> 12) & 0x1FF;

    struct page_table* pdpt = get_next_level(vmm->pml4, vmm, pmm, pml4_idx, flags);
    struct page_table* pd   = get_next_level(pdpt, vmm, pmm, pdpt_idx, flags);
    struct page_table* pt   = get_next_level(pd, vmm, pmm, pd_idx, flags);

    pt->entries[pt_idx] = physical_addr | flags | PTE_PRESENT;
}