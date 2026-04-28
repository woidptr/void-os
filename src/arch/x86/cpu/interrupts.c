#include "core/hal/interrupts.h"
#include "kernel.h"

void interrupts_enable() {
    __asm__ volatile ("sti");
}

void interrupts_disable() {
    __asm__ volatile ("cli");
}

void interrupts_register(struct kernel_ctx* kctx, uint8_t vector, interrupt_handler_t handler) {
    idt_set_handler(&kctx->arch.idt, vector, handler);
}