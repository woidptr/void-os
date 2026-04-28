#pragma once
#include <stdint.h>
#include <stddef.h>

struct smp_ctx {
    uint32_t core_count;
    uint8_t  bsp_apic_id;
    uint8_t  apic_ids[256];
};

/**
 * @brief Parses ACPI tables to identify available CPU cores and the BSP APIC ID.
 *
 * This function locates the MADT (Multiple APIC Description Table) using the 
 * provided RSDP pointer. It iterates through the table entries to discover 
 * Processor Local APIC structures, populating the context with enabled core 
 * IDs. It also utilizes the CPUID instruction to identify the Local APIC ID 
 * of the core currently executing the code (the BSP).
 *
 * @param out_ctx     Pointer to the SMP context structure to be populated.
 * @param rsdp_ptr    Pointer to the ACPI Root System Description Pointer.
 * @param hhdm_offset The virtual memory offset for the Higher Half Direct Map.
 */
void parse_smp(struct smp_ctx* out_ctx, void* rsdp_ptr, uint64_t hhdm_offset);