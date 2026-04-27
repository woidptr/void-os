#pragma once
#include <stdint.h>

struct kernel_ctx;

void cpu_halt();

/**
 * @brief Initializes the Bootstrap Processor (BSP) and system-wide CPU structures.
 *
 * This function must be called exactly once during early kernel initialization 
 * by the BSP (typically Core 0). It is responsible for allocating multi-core 
 * memory structures (like the GDT arrays) and applying the security mechanisms 
 * to the BSP itself.
 *
 * @param kctx Pointer to the master kernel state structure.
 */
void cpu_init_bsp(struct kernel_ctx* kctx, uint64_t hhdm_offset);

/**
 * @brief Secures and initializes an Application Processor (AP) upon wake-up.
 * 
 * This function must be called by every secondary core immediately after it 
 * exits the 16-bit trampoline and enters 64-bit C code. It applies the 
 * security mechanisms (like loading the GDT/IDT) for that specific core 
 * using the memory previously allocated by the BSP.
 * 
 * @param kctx Pointer to the master kernel state structure.
 * @param core_id The logical, zero-indexed ID of the waking core.
 */
void cpu_init_ap(struct kernel_ctx* kctx, uint32_t core_id);