#include "core/hal/cpu.h"
#include "gdt.h"
#include "kernel.h"

void cpu_init_bsp(struct kernel_ctx* kctx, uint64_t hhdm_offset) {
    gdt_init_system(&kctx->arch.gdt, kctx->smp.core_count, &kctx->pmm, hhdm_offset);
    gdt_init_core(&kctx->arch.gdt, 0);
}

void cpu_init_ap(struct kernel_ctx* kctx, uint32_t core_id) {
    gdt_init_core(&kctx->arch.gdt, core_id);
}