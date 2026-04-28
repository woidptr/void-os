#include "kernel.h"
#include "core/hal/cpu.h"

extern void executive_main(struct kernel_ctx* kctx);

void kernel_main(struct boot_info* info) {
    struct kernel_ctx kctx = {0};

    // I don't fucking know if I even need this
    kctx.framebuffer = info->screen;

    pmm_init(&kctx, info->memory_map, info->hhdm_offset);

    parse_smp(&kctx.smp, info->rsdp, info->hhdm_offset);
    cpu_init_bsp(&kctx, info->hhdm_offset);

    vmm_init(&kctx, info);
    heap_init(&kctx);

    // __asm__ volatile ("int $3");

    executive_main(&kctx);
}