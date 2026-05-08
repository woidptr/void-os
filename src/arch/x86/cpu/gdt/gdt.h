#pragma once
#include <stdint.h>
#include <stddef.h>

struct pmm_ctx;

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t flags;
    uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

struct gdt_ctx {
    struct gdt_entry (*entries)[5];
    struct gdt_ptr* pointers;
};

void gdt_init_system(struct gdt_ctx* ctx, uint32_t core_count, struct pmm_ctx* pmm, uint64_t hhdm_offset);
void gdt_init_core(struct gdt_ctx* ctx, uint32_t core_id);