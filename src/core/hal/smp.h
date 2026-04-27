#pragma once
#include <stdint.h>
#include <stddef.h>

struct smp_ctx {
    uint32_t core_count;
    uint8_t  bsp_apic_id;
    uint8_t  apic_ids[256];
};

void parse_smp(struct smp_ctx* out_ctx, void* rsdp_ptr, uint64_t hhdm_offset);