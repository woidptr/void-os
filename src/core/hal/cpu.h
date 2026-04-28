#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct kernel_ctx;

void cpu_halt();

/**
 * @brief Permanently locks the current CPU core in a halted state.
 *
 * This function places the processor into a continuous low-power sleep state. 
 * It wraps the x86 'hlt' instruction in an infinite loop to guarantee that 
 * even if a Non-Maskable Interrupt (NMI) forces the CPU to wake up, it will 
 * immediately halt again upon returning. It is primarily used at the end of 
 * a kernel panic to freeze the system and prevent further data corruption.
 */
void cpu_lock();

/**
 * @brief Initializes the Bootstrap Processor (BSP) architecture structures.
 *
 * This function must be called exactly once during early kernel boot by the 
 * BSP (typically Core 0). It acts as the central factory for all hardware-level 
 * CPU features. It allocates and configures the system-wide Global Descriptor 
 * Table (GDT) and Interrupt Descriptor Table (IDT), and then immediately loads 
 * these structures into the BSP's hardware registers to secure the core and 
 * catch exceptions.
 *
 * @param kctx        Pointer to the master kernel state structure.
 * @param hhdm_offset The Higher Half Direct Map virtual offset, required for 
 *                    safely mapping the Task State Segment (TSS) and interrupt stacks.
 */
void cpu_init_bsp(struct kernel_ctx* kctx, uint64_t hhdm_offset);

/**
 * @brief Initializes an Application Processor (AP) architecture state upon wake-up.
 *
 * This function must be called by every secondary core immediately after it 
 * exits the early boot trampoline and enters 64-bit C code. Unlike the BSP, 
 * secondary cores do not allocate or generate new system structures. They 
 * simply load the pre-allocated Global Descriptor Table (GDT) and Interrupt 
 * Descriptor Table (IDT) pointers into their local CPU registers to secure 
 * the core's memory boundaries and enable exception catching.
 *
 * @param kctx    Pointer to the master kernel state structure.
 * @param core_id The logical, zero-indexed ID of the waking core.
 */
void cpu_init_ap(struct kernel_ctx* kctx, uint32_t core_id);

#ifdef __cplusplus
}
#endif