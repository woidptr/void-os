#pragma once
#include <stdint.h>
#include <stddef.h>
#include "core/hal/interrupts.h"

enum cpu_exception {
    EXCEPTION_DIVIDE_BY_ZERO = 0,
    EXCEPTION_DEBUG = 1,
    EXCEPTION_NMI = 2,
    EXCEPTION_BREAKPOINT = 3,
    EXCEPTION_OVERFLOW = 4,
    EXCEPTION_BOUND_RANGE = 5,
    EXCEPTION_INVALID_OPCODE = 6,
    EXCEPTION_DEVICE_NOT_AVAILABLE = 7,
    EXCEPTION_DOUBLE_FAULT = 8,
    EXCEPTION_COPROCESSOR_SEGMENT_OVERRUN = 9,
    EXCEPTION_INVALID_TSS = 10,
    EXCEPTION_SEGMENT_NOT_PRESENT = 11,
    EXCEPTION_STACK_SEGMENT_FAULT = 12,
    EXCEPTION_GENERAL_PROTECTION = 13,
    EXCEPTION_PAGE_FAULT = 14,
    EXCEPTION_X87_FLOATING_POINT = 16,
    EXCEPTION_ALIGNMENT_CHECK = 17,
    EXCEPTION_MACHINE_CHECK = 18,
    EXCEPTION_SIMD_FLOATING_POINT = 19,
    EXCEPTION_VIRTUALIZATION = 20,
    EXCEPTION_CONTROL_PROTECTION = 21,
    EXCEPTION_HYPERVISOR_INJECTION = 28,
    EXCEPTION_VMM_COMMUNICATION = 29,
    EXCEPTION_SECURITY = 30,

    // end marker, not a real exception
    EXCEPTION_MAX_RESERVED = 31,
};

struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t attributes;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

struct idt_ctx {
    struct idt_entry entries[256];
    struct idt_ptr pointer;
    interrupt_handler_t drivers[256];
};

void idt_init_system(struct idt_ctx* idt);
void idt_init_core(struct idt_ctx* idt);
void idt_set_handler(struct idt_ctx* idt, uint8_t vector, interrupt_handler_t handler);
void idt_handler(struct idt_ctx* idt, struct interrupt_frame* frame);