#include "core/hal/mmu.h"
#include "lib/cmem.h"
#include "core/mm/vmm.h"

#define PTE_PRESENT  (1ull << 0)
#define PTE_WRITABLE (1ull << 1)
#define PTE_USER     (1ull << 2)
#define PTE_GET_FRAME(entry) ((entry) & 0x000FFFFFFFFFF000ull)

void* mmu_create_directory(struct pmm_ctx* pmm, uint64_t hhdm_offset) {
    void* phys = pmm_alloc_page(pmm);
    if (!phys) return nullptr;

    void* virt = (void*)((uint64_t)phys + hhdm_offset);
    memset(virt, 0, 4096);
    return virt;
}

static struct page_table* get_next_level(struct page_table* current_level, size_t index, uint64_t flags, struct pmm_ctx* pmm, uint64_t hhdm) {
    uint64_t entry = current_level->entries[index];

    if ((entry & PTE_PRESENT) != 0) {
        return (struct page_table*)(PTE_GET_FRAME(entry) + hhdm);
    }

    void* new_page = pmm_alloc_page(pmm);
    if (!new_page) for(;;) __asm__("hlt");

    struct page_table* new_table = (struct page_table*)((uint64_t)new_page + hhdm);
    memset(new_table, 0, 4096);

    current_level->entries[index] = (uint64_t)new_page | flags | PTE_PRESENT | PTE_WRITABLE;
    return new_table;
}

void mmu_map_page(void* directory, uint64_t virt, uint64_t phys, uint32_t flags, struct pmm_ctx* pmm, uint64_t hhdm_offset) {

}

void mmu_switch_directory(void* directory_phys) {
    __asm__ volatile (
        "mov %0, %%cr3"
        :
        : "r" ((uint64_t)directory_phys)
        : "memory"
    );
}