#include <stdint.h>
#include "core/pmm.h"
#include "core/vmm.h"
#include "core/heap.h"
#include "boot/boot.h"
#include "core/hal/smp.h"
#include <arch.h>

struct kernel_ctx {
    struct framebuffer_ctx* framebuffer;
    struct pmm_ctx pmm;
    struct smp_ctx smp;
    struct arch_state arch;
    struct vmm_ctx vmm;
    struct heap_ctx heap;
};

void kernel_main(struct boot_info* info);