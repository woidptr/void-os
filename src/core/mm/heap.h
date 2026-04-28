#pragma once
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct kernel_ctx;
struct pmm_ctx;

#define HEAP_BUCKET_COUNT 9

struct heap_bucket {
    size_t size;
    void* free_list;
    size_t pages_used;
};

struct heap_ctx {
    struct pmm_ctx* pmm;
    struct vmm_ctx* vmm;

    uint64_t heap_base;
    uint64_t current_top;

    struct heap_bucket buckets[HEAP_BUCKET_COUNT];
};

void  heap_init(struct kernel_ctx* heap);
void* heap_alloc(struct heap_ctx* heap, size_t size);
void  heap_free(struct heap_ctx* heap, void* ptr);

#ifdef __cplusplus
}
#endif