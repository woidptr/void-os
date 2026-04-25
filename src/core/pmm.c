#include "pmm.h"
#include <stddef.h>
#include "lib/cmem.h"
#include "core/hal/cpu.h"
#include "kernel.h"

#define align_up(value, alignment) (((value) + (alignment) - 1) & ~((alignment) - 1))

void pmm_init(struct kernel_ctx* kctx, struct memmap_ctx* memmap, uint64_t hhdm_offset) {
    struct pmm_ctx* pmm = &kctx->pmm;

    uint64_t top_address = 0;

    for (size_t i = 0; i < memmap->entry_count; i++) {
        struct memmap_entry* entry = &memmap->entries[i];
        uint64_t entry_top = entry->base + entry->length;
        if (entry_top > top_address) {
            top_address = entry_top;
        }
    }

    pmm->total_pages = top_address / PAGE_SIZE;
    pmm->bitmap_size = align_up(pmm->total_pages / 8, PAGE_SIZE);
    pmm->free_pages = 0;
    pmm->last_allocated_index = 0;

    pmm->bitmap = nullptr;
    for (size_t i = 0; i < memmap->entry_count; i++) {
        struct memmap_entry* entry = &memmap->entries[i];

        if (entry->type == MEMMAP_USABLE && entry->length >= pmm->bitmap_size) {
            pmm->bitmap = (uint8_t*)(entry->base + hhdm_offset);

            entry->base += pmm->bitmap_size;
            entry->length -= pmm->bitmap_size;
            break;
        }
    }

    if (pmm->bitmap == nullptr) {
        cpu_halt();
    }

    memset(pmm->bitmap, 0xFF, pmm->bitmap_size);

    for (size_t i = 0; i < memmap->entry_count; i++) {
        struct memmap_entry* entry = &memmap->entries[i];

        if (entry->type == MEMMAP_USABLE) {
            uint64_t start_page = entry->base / PAGE_SIZE;
            uint64_t end_page = (entry->base + entry->length) / PAGE_SIZE;

            for (uint64_t p = start_page; p < end_page; p++) {
                bitmap_clear(pmm->bitmap, p);
                pmm->free_pages++;
            }
        }
    }
}

void* pmm_alloc_page(struct pmm_ctx* pmm) {
    for (size_t i = pmm->last_allocated_index; i < pmm->total_pages; i++) {
        if (!bitmap_test(pmm->bitmap, i)) {
            bitmap_set(pmm->bitmap, i);
            pmm->free_pages--;
            pmm->last_allocated_index = i;

            return (void*)(i * PAGE_SIZE);
        }
    }

    for (size_t i = 0; i < pmm->last_allocated_index; i++) {
        if (!bitmap_test(pmm->bitmap, i)) {
            bitmap_set(pmm->bitmap, i);
            pmm->free_pages--;
            pmm->last_allocated_index = i;

            return (void*)(i * PAGE_SIZE);
        }
    }

    return nullptr;
}

void pmm_free_page(struct pmm_ctx* pmm, void* ptr) {
    if (ptr == nullptr) return;

    uint64_t page_index = (uint64_t)ptr / PAGE_SIZE;

    if (bitmap_test(pmm->bitmap, page_index)) {
        bitmap_clear(pmm->bitmap, page_index);
        pmm->free_pages++;

        if (page_index < pmm->last_allocated_index) {
            pmm->last_allocated_index = page_index;
        }
    }
}