#include "memory.h"

extern volatile struct limine_memmap_request memmap_request;
extern volatile struct limine_hhdm_request hhdm_request;

typedef struct heap_context {
    uint8_t* start;
    size_t size;
    size_t used;
} heap_context_t;

heap_context_t heap_ctx;

void memory_init() {
    struct limine_memmap_response* memmap = memmap_request.response;
    struct limine_hhdm_response* hhdm = hhdm_request.response;
    if (memmap == nullptr || hhdm == nullptr) {
        return;
    }

    uint64_t virtual_offset = hhdm->offset;

    for (uint64_t i = 0; i < memmap->entry_count; i++) {
        struct limine_memmap_entry* entry = memmap->entries[i];

        if (entry->type == LIMINE_MEMMAP_USABLE) {
            if (entry->length > heap_ctx.size) {
                heap_ctx.start = (uint8_t*)(entry->base + virtual_offset);
                heap_ctx.size = entry->length;
            }
        }
    }
}

void* malloc(size_t size) {
    if (size == 0) return nullptr;

    size = (size + 7) & ~7;

    if (heap_ctx.used + size > heap_ctx.size) {
        return nullptr;
    }

    void* allocated_memory = heap_ctx.start + heap_ctx.used;

    heap_ctx.used += size;

    return allocated_memory;
}

void* memcpy(void* dest, const void* src, size_t count) {
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;

    for (size_t i = 0; i < count; i++) {
        d[i] = s[i];
    }

    return dest;
}