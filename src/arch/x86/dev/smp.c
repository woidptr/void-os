#include "core/hal/smp.h"
#include "core/acpi.h"

void parse_smp(struct smp_ctx* out_ctx, void* rsdp_ptr, uint64_t hhdm_offset) {
    out_ctx->core_count = 0;
    out_ctx->bsp_apic_id = 0;

    struct acpi_header* madt = acpi_find_table(rsdp_ptr, hhdm_offset, "APIC");

    if (!madt) {
        out_ctx->core_count = 1;
        return;
    }

    uint8_t* ptr = (uint8_t*)madt + 44;
    uint8_t* end = (uint8_t*)madt + madt->length;

    while (ptr < end) {
        uint8_t type = ptr[0];
        uint8_t length = ptr[1];

        if (type == 0) {
            uint8_t apic_id = ptr[3];
            uint32_t flags = *(uint32_t*)(ptr + 4);

            if (flags & 1) {
                if (out_ctx->core_count < 256) {
                    out_ctx->apic_ids[out_ctx->core_count] = apic_id;
                    out_ctx->core_count++;
                }
            }
        }
        ptr += length;
    }

    uint32_t ebx;
    __asm__ volatile ("cpuid" : "=b"(ebx) : "a"(1) : "ecx", "edx");

    // bits from 24 to 31 is apic id info
    out_ctx->bsp_apic_id = (ebx >> 24) & 0xFF;
}