#include "heap.h"
#include "pmm.h"
#include "vmm.h"
#include "kernel.h"

#define HEAP_START_VIRTUAL 0xFFFFC00000000000

struct alloc_header {
    size_t size;
};

struct free_node {
    struct free_node* next;
};

void heap_init(struct kernel_ctx* kctx) {
    struct heap_ctx* heap = &kctx->heap;

    heap->pmm = &kctx->pmm;
    heap->vmm = &kctx->vmm;

    heap->heap_base = HEAP_START_VIRTUAL;
    heap->current_top = HEAP_START_VIRTUAL;

    size_t current_size = 16;
    for (int i = 0; i < HEAP_BUCKET_COUNT; i++) {
        heap->buckets[i].size = current_size;
        heap->buckets[i].free_list = nullptr;
        heap->buckets[i].pages_used = 0;

        current_size *= 2;
    }
}

void* heap_alloc(struct heap_ctx* heap, size_t size) {
    if (size == 0) return nullptr;

    size_t total_size = size + sizeof(struct alloc_header);

    int bucket_index = -1;
    for (int i = 0; i < HEAP_BUCKET_COUNT; i++) {
        if (heap->buckets[i].size >= total_size) {
            bucket_index = i;
            break;
        }
    }

    if (bucket_index == -1) {
        return nullptr;
    }

    struct heap_bucket* bucket = &heap->buckets[bucket_index];

    if (bucket->free_list == nullptr) {
        void* phys_frame = pmm_alloc_page(heap->pmm);
        if (!phys_frame) {
            return nullptr;
        }

        uint64_t virt_addr = heap->current_top;
        heap->current_top += PAGE_SIZE;

        uint64_t flags = PTE_PRESENT | PTE_WRITABLE | PTE_NO_EXECUTE;
        vmm_map_page(heap->vmm, heap->pmm, virt_addr, (uint64_t)phys_frame, flags);

        void* new_page = (void*)virt_addr;
        bucket->pages_used++;

        size_t chunks = PAGE_SIZE / bucket->size;
        for (size_t i = 0; i < chunks; i++) {
            struct free_node* block = (struct free_node*)((uintptr_t)new_page + (i * bucket->size));
            block->next = bucket->free_list;
            bucket->free_list = block;
        }
    }

    struct free_node* node = bucket->free_list;
    bucket->free_list = node->next;

    struct alloc_header* header = (struct alloc_header*)node;
    header->size = bucket_index;

    return (void*)((uintptr_t)header + sizeof(struct alloc_header));
}

void heap_free(struct heap_ctx* heap, void* ptr) {
    if (!ptr) return;

    struct alloc_header* header = (struct alloc_header*)((uintptr_t)ptr - sizeof(struct alloc_header));

    size_t bucket_index = header->size;

    if (bucket_index >= HEAP_BUCKET_COUNT) {
        return;
    }

    struct heap_bucket* bucket = &heap->buckets[bucket_index];

    struct free_node* node = (struct free_node*)header;

    node->next = bucket->free_list;
    bucket->free_list = node;
}