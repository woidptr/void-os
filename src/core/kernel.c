#include "kernel.h"
#include "core/hal/cpu.h"
#include "drivers/gfx/backends.h"
#include "core/format.h"

extern void executive_main(struct kernel_ctx* kctx);

void kernel_main(struct boot_info* info) {
    struct kernel_ctx kctx = {0};

    pmm_init(&kctx, info->memory_map, info->hhdm_offset);
    vmm_init(&kctx, info);
    heap_init(&kctx);

    // format("Test {}", 1);

    softgfx_init(&kctx, info->screen->addr, info->screen->width, info->screen->height);

    gfx_clear(&kctx.gfx, GFX_CLEAR_COLOR | GFX_CLEAR_DEPTH, 0xFFFFFFFF, 1.0f);
    gfx_swap_buffers(&kctx.gfx);

    // parse_smp(&kctx.smp, info->rsdp, info->hhdm_offset);
    // cpu_init_bsp(&kctx, info->hhdm_offset);

    // __asm__ volatile ("int $3");

    executive_main(&kctx);
}