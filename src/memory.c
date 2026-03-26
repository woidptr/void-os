#include "memory.h"

extern volatile struct limine_memmap_request memmap_request;
// extern volatile struct limine_hhdm_request hhdm_request;

typedef struct block_header {
    size_t size;
    bool is_free;
    struct block_header* next;
} block_header_t;

static block_header_t* heap_start_block = nullptr;

typedef struct heap_context {
    uint8_t* start;
    size_t size;
    size_t used;
} heap_context_t;

heap_context_t heap_ctx;

void memory_init(runtime_context_t* runtime_ctx) {
    struct limine_memmap_response* memmap = memmap_request.response;
    if (memmap == nullptr) {
        return;
    }

    uint64_t virtual_offset = runtime_ctx->hhdm_offset;

    for (uint64_t i = 0; i < memmap->entry_count; i++) {
        struct limine_memmap_entry* entry = memmap->entries[i];

        if (entry->type == LIMINE_MEMMAP_USABLE) {
            if (entry->length > heap_ctx.size) {
                heap_ctx.start = (uint8_t*)(entry->base + virtual_offset);
                heap_ctx.size = entry->length;
            }
        }
    }

    heap_start_block = (block_header_t*)heap_ctx.start;
    heap_start_block->size = heap_ctx.size - sizeof(block_header_t);
    heap_start_block->is_free = true;
    heap_start_block->next = nullptr;
}

void* malloc(size_t size) {
    if (size == 0) return nullptr;

    size = (size + 7) & ~7;

    block_header_t* current = heap_start_block;

    while (current != nullptr) {
        if (current->is_free && current->size >= size) {
            if (current->size >= size + sizeof(block_header_t) + 8) {
                block_header_t* new_block = (block_header_t*)((uint8_t*)current + sizeof(block_header_t) + size);
                new_block->size = current->size - size - sizeof(block_header_t);
                new_block->is_free = true;
                new_block->next = current->next;

                current->size = size;
                current->next = new_block;
            }
            current->is_free = false;

            return (void*)(current + 1);
        }
        current = current->next;
    }

    return nullptr;

    // if (heap_ctx.used + size > heap_ctx.size) {
    //     return nullptr;
    // }

    // void* allocated_memory = heap_ctx.start + heap_ctx.used;

    // heap_ctx.used += size;

    // return allocated_memory;
}

void* memcpy(void* dest, const void* src, size_t count) {
    uint64_t* d64 = (uint64_t*)dest;
    const uint64_t* s64 = (const uint64_t*)src;

    size_t blocks = count / 8;
    for (size_t i = 0; i < blocks; i++) {
        d64[i] = s64[i];
    }

    uint8_t* d = (uint8_t*)(d64 + blocks);
    const uint8_t* s = (const uint8_t*)(s64 + blocks);

    for (size_t i = 0; i < (count % 8); i++) {
        d[i] = s[i];
    }

    return dest;
}

void memfree(void* ptr) {
    if (ptr == nullptr) return;

    block_header_t* header = (block_header_t*)ptr - 1;
    header->is_free = true;

    if (header->next != nullptr && header->next->is_free) {
        header->size += sizeof(block_header_t) + header->next->size;
        header->next = header->next->next;
    }
}