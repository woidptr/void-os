#include "core/hal/ints.h"
#include "core/kernel.h"

void ints_register(struct kernel_ctx* kctx, uint8_t vector, interrupt_handler_t handler) {
    idt_set_handler(&kctx->arch.idt, vector, handler);
}