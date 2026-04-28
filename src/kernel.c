#include "kernel.h"
#include "core/pmm.h"
#include "core/hal/cpu.h"

void kernel_main(struct boot_info* info) {
    struct kernel_ctx kctx = {0};

    // I don't fucking know if I even need this
    kctx.framebuffer = info->screen;

    pmm_init(&kctx, info->memory_map, info->hhdm_offset);
    parse_smp(&kctx.smp, info->rsdp, info->hhdm_offset);

    cpu_init_bsp(&kctx, info->hhdm_offset);

    // __asm__ volatile ("int $3");

    // Broken shit
    // vmm_init(&kctx, info);
    // heap_init(&kctx);

    cpu_halt();
}