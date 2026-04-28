#pragma once
#include <stdint.h>

struct kernel_ctx;

struct interrupt_frame {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;
    uint64_t vector;
    uint64_t error_code;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

typedef void (*interrupt_handler_t)(struct interrupt_frame* frame);

/**
 * @brief Globally enables hardware interrupts on the current core.
 * 
 * This function allows the CPU to respond to external hardware signals 
 * (like timer pulses or keyboard events). It should be called after the 
 * IDT is fully initialized.
 */
void interrupts_enable();

/**
 * @brief Globally disables hardware interrupts on the current core.
 * 
 * This function prevents the CPU from being interrupted by hardware events. 
 * It is commonly used to protect "critical sections" of code where 
 * atomicity is required.
 */
void interrupts_disable();

/**
 * @brief Registers a software handler for a specific interrupt vector.
 * 
 * This function binds a C function to a hardware or software interrupt. 
 * When the specified vector fires, the CPU will jump through the 
 * architecture-specific gates (IDT) and execute the provided handler.
 * 
 * @param kctx    Pointer to the master kernel state structure.
 * @param vector  The 8-bit interrupt vector number (0-255).
 * @param handler Pointer to the C function that will process the interrupt.
 */
void interrupts_register(struct kernel_ctx* kctx, uint8_t vector, interrupt_handler_t handler);