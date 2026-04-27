#include <limine.h>
#include <stddef.h>
#include "boot.h"
#include "kernel.h"

__attribute__((used, section(".limine_requests")))
static volatile uint64_t requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests")))
static volatile uint64_t base_revision[] = { LIMINE_BASE_REVISION(3) };

__attribute__((used, section(".limine_requests")))
volatile struct limine_memmap_request memmap_req = {
    .id = LIMINE_MEMMAP_REQUEST_ID,
    .revision = 0,
};

__attribute__((used, section(".limine_requests")))
volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST_ID,
    .revision = 0,
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request fb_req = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_executable_address_request exe_addr_req = {
    .id = LIMINE_EXECUTABLE_ADDRESS_REQUEST_ID,
    .revision = 0,
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request rsdp_req = {
    .id = LIMINE_RSDP_REQUEST_ID,
    .revision = 0,
};

__attribute__((used, section(".limine_requests")))
static volatile uint64_t requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

void _start() {
    if (!memmap_req.response || !hhdm_request.response) {
        for (;;) __asm__ ("hlt");
    }

    struct limine_memmap_response* limine_map = memmap_req.response;

    struct memmap_ctx mm_ctx;
    mm_ctx.entry_count = 0;

    for (size_t i = 0; i < limine_map->entry_count; i++) {
        if (i >= MAX_MEMMAP_ENTRIES) break;

        struct limine_memmap_entry* l_entry = limine_map->entries[i];
        struct memmap_entry* m_entry = &mm_ctx.entries[i];

        m_entry->base = l_entry->base;
        m_entry->length = l_entry->length;

        switch (l_entry->type) {
            case LIMINE_MEMMAP_USABLE: {
                m_entry->type = MEMMAP_USABLE;
                break;
            }
            case LIMINE_MEMMAP_RESERVED: {
                m_entry->type = MEMMAP_RESERVED;
                break;
            }
            case LIMINE_MEMMAP_ACPI_RECLAIMABLE: {
                m_entry->type = MEMMAP_ACPI_RECLAIM;
                break;
            }
            case LIMINE_MEMMAP_ACPI_NVS: {
                m_entry->type = MEMMAP_ACPI_NVS;
                break;
            }
            case LIMINE_MEMMAP_BAD_MEMORY: {
                m_entry->type = MEMMAP_BAD_MEMORY;
                break;
            }
            case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE: {
                m_entry->type = MEMMAP_BOOTLOADER;
                break;
            }
            case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES: {
                m_entry->type = MEMMAP_KERNEL;
                break;
            }
            default: {
                m_entry->type = MEMMAP_RESERVED;
                break;;
            }
        }

        mm_ctx.entry_count++;
    }

    struct framebuffer_ctx fb_ctx;
    if (fb_req.response && fb_req.response->framebuffer_count > 0) {
        struct limine_framebuffer* l_fb = fb_req.response->framebuffers[0];
        fb_ctx.addr = l_fb->address;
        fb_ctx.width = l_fb->width;
        fb_ctx.height = l_fb->height;
        fb_ctx.pitch = l_fb->pitch;
    }

    struct boot_info info = {
        .memory_map = &mm_ctx,
        .hhdm_offset = hhdm_request.response->offset,
        .screen = &fb_ctx,
        .rsdp = rsdp_req.response->address,
        .kernel_physical_base = exe_addr_req.response->physical_base,
        .kernel_virtual_base = exe_addr_req.response->virtual_base,
    };

    kernel_main(&info);
}