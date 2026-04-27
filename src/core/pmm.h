#pragma once
#include <stdint.h>
#include <stddef.h>
#include <config.h>

typedef uint8_t bitmap_block_t;

#define bitmap_empty ((bitmap_block_t)0)
#define bitmap_full  ((bitmap_block_t)~0)
#define BITMAP_FULL 0xFF

struct kernel_ctx;
struct memmap_ctx;

struct pmm_ctx {
    uint8_t* bitmap;
    size_t bitmap_size;
    size_t total_pages;
    size_t free_pages;
    size_t last_allocated_index;
    uint64_t hhdm_offset;
};

static inline void bitmap_set(uint8_t* bitmap, size_t bit) {
    bitmap[bit / 8] |= (1 << (bit % 8));
}

static inline void bitmap_clear(uint8_t* bitmap, size_t bit) {
    bitmap[bit / 8] &= ~(1 << (bit % 8));
}

static inline bool bitmap_test(const uint8_t* bitmap, size_t bit) {
    return (bitmap[bit / 8] & (1 << (bit % 8))) != 0;
}

void  pmm_init(struct kernel_ctx* kctx, struct memmap_ctx* memmap, uint64_t hhdm_offset);
void* pmm_alloc_page(struct pmm_ctx* pmm);
void* pmm_alloc_pages(struct pmm_ctx* pmm, size_t count);
void  pmm_free_page(struct pmm_ctx* pmm, void* ptr);