#pragma once
#include <stdint.h>
#include "core/mm/pmm.h"
#include "core/mm/vmm.h"
#include "core/mm/heap.h"
#include "boot/boot.h"
#include "core/hal/smp.h"
#include "core/graphics/gfx.h"
#include <arch.h>

struct kernel_ctx {
    struct pmm_ctx pmm;
    struct smp_ctx smp;
    struct arch_state arch;
    struct vmm_ctx vmm;
    struct heap_ctx heap;
    struct gfx_ctx gfx;
};

/**
 * @brief The primary C entry point of the operating system.
 * 
 * This function bootstraps the foundational subsystems of the kernel in a strict,
 * dependency-safe order: Physical Memory -> Virtual Memory -> Heap -> Hardware Discovery.
 * Once the environment is stable and the Boot Strap Processor (BSP) is configured, 
 * it hands control over to the C++ Executive subsystem.
 * 
 * @param info Pointer to the sanitized, architecture-agnostic boot information.
 *             Must be fully populated by the bootloader shim before invocation.
 * 
 * @warning The memory managers (PMM, VMM, Heap) MUST be fully initialized before 
 *          any hardware parsing (SMP, ACPI) is attempted, as those parsers rely 
 *          on dynamic memory allocation to store core configurations.
 */
void kernel_main(struct boot_info* info);