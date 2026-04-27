#include "acpi.h"

static bool sig_match(const char* a, const char* b) {
    return a[0] == b[0] && a[1]==b[1] && a[2] == b[2] && a[3] == b[3];
}

void* acpi_find_table(void* rsdp_ptr, uint64_t hhdm_offset, const char* sig) {
    if (!rsdp_ptr) return nullptr;

    uint8_t revision = *((uint8_t*)rsdp_ptr + 15);

    int entries;
    uint8_t* table_ptrs;
    int ptr_size;

    if (revision == 0) {
        // ACPI 1.0 (QEMU Default / Legacy BIOS) -> Use RSDT
        // The 32-bit physical address is at offset 16
        uint32_t rsdt_phys = *(uint32_t*)((uint8_t*)rsdp_ptr + 16);
        struct acpi_header* rsdt = (struct acpi_header*)((uint64_t)rsdt_phys + hhdm_offset);
        
        // RSDT uses 32-bit (4 byte) pointers
        entries = (rsdt->length - sizeof(struct acpi_header)) / 4;
        table_ptrs = (uint8_t*)rsdt + sizeof(struct acpi_header);
        ptr_size = 4;
    } else {
        // ACPI 2.0+ (UEFI) -> Use XSDT
        // The 64-bit physical address is at offset 24
        uint64_t xsdt_phys = *(uint64_t*)((uint8_t*)rsdp_ptr + 24);
        struct acpi_header* xsdt = (struct acpi_header*)(xsdt_phys + hhdm_offset);
        
        // XSDT uses 64-bit (8 byte) pointers
        entries = (xsdt->length - sizeof(struct acpi_header)) / 8;
        table_ptrs = (uint8_t*)xsdt + sizeof(struct acpi_header);
        ptr_size = 8;
    }

    for (int i = 0; i < entries; i++) {
        struct acpi_header* table = (struct acpi_header*)(table_ptrs[i] + hhdm_offset);
        if (sig_match(table->signature, sig)) {
            return table;
        }
    }

    return nullptr;
}