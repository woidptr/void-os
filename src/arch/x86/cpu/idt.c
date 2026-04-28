#include "idt.h"
#include "core/panic.h"

extern uintptr_t isr_stub_table[];

static void idt_set_gate(struct idt_ctx* idt, uint8_t vector, void* isr_ptr, uint8_t attributes) {
    uint64_t isr_addr = (uint64_t)isr_ptr;

    idt->entries[vector].offset_low = (uint16_t)(isr_addr & 0xFFFF);
    idt->entries[vector].selector = 0x08;
    idt->entries[vector].ist = 0;
    idt->entries[vector].attributes = attributes;
    idt->entries[vector].offset_mid = (uint16_t)((isr_addr >> 16) & 0xFFFF);
    idt->entries[vector].offset_high = (uint32_t)(isr_addr >> 32);
    idt->entries[vector].zero = 0;
}

void idt_init_system(struct idt_ctx* idt) {
    idt->pointer.limit = (sizeof(struct idt_entry) * 256) - 1;
    idt->pointer.base = (uint64_t)&idt->entries[0];

    for (int i = 0; i < 256; i++) {
        idt->drivers[i] = nullptr;
        idt_set_gate(idt, i, (void*)isr_stub_table[i], 0x8E);
    }
}

void idt_init_core(struct idt_ctx* idt) {
    __asm__ volatile ("lidt %0" : : "m"(idt->pointer));
}

void idt_set_handler(struct idt_ctx* idt, uint8_t vector, interrupt_handler_t handler) {
    idt->drivers[vector] = handler;
}

void idt_handler(struct idt_ctx* idt, struct interrupt_frame* frame) {
    interrupt_handler_t driver = idt->drivers[frame->vector];

    if (driver != nullptr) {
        driver(frame);
    } else {
        if (frame->vector < EXCEPTION_MAX_RESERVED) {
            kernel_panic();
        }
    }
}