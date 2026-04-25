#include "kernel.h"
#include "core/pmm.h"

void kernel_main(struct boot_info* info) {
    struct kernel_ctx kctx = {0};

    kctx.framebuffer = info->screen;

    pmm_init(&kctx, info->memory_map, info->hhdm_offset);
    vmm_init(&kctx, info);
    // heap_init(&kctx);

    for (;;) __asm__("hlt");
}