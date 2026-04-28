#include <stdint.h>
#include "gdt.h"
#include "core/pmm.h"
#include "lib/cmem.h"
#include <config.h>

extern void gdt_load(struct gdt_ptr* ptr);

static void gdt_set_gate(struct gdt_ctx* ctx, uint32_t core_id, int num, uint8_t access, uint8_t flags) {
    ctx->entries[core_id][num].limit_low   = 0;
    ctx->entries[core_id][num].base_low    = 0;
    ctx->entries[core_id][num].base_middle = 0;
    ctx->entries[core_id][num].access      = access;
    ctx->entries[core_id][num].flags       = (flags & 0xF0);
    ctx->entries[core_id][num].base_high   = 0;
}

void gdt_init_system(struct gdt_ctx* ctx, uint32_t core_count, struct pmm_ctx* pmm, uint64_t hhdm_offset) {
    size_t gdt_size = core_count * (sizeof(struct gdt_entry) * 5);
    size_t gp_size = core_count * sizeof(struct gdt_ptr);
    size_t total_size = gdt_size + gp_size;
    size_t pages_needed = (total_size + PAGE_SIZE - 1) / PAGE_SIZE;

    void* phys_ptr = pmm_alloc_pages(pmm, pages_needed);

    uint64_t virt_ptr = (uint64_t)phys_ptr + hhdm_offset;

    memset((void*)virt_ptr, 0, total_size);

    ctx->entries = (struct gdt_entry(*)[5])virt_ptr;
    ctx->pointers = (struct gdt_ptr*)(virt_ptr + gdt_size);
}

void gdt_init_core(struct gdt_ctx* ctx, uint32_t core_id) {
    ctx->pointers[core_id].limit = (sizeof(struct gdt_entry) * 5) - 1;

    ctx->pointers[core_id].base = (uintptr_t)&ctx->entries[core_id];

    gdt_set_gate(ctx, core_id, 0, 0, 0);
    gdt_set_gate(ctx, core_id, 1, 0x9A, 0x20);
    gdt_set_gate(ctx, core_id, 2, 0x92, 0x00);
    gdt_set_gate(ctx, core_id, 3, 0xF2, 0x00);
    gdt_set_gate(ctx, core_id, 4, 0xFA, 0x20);

    gdt_load(&ctx->pointers[core_id]);
}