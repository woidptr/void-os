#pragma once
#include "arch/x86/cpu/gdt/gdt.h"
#include "arch/x86/cpu/idt/idt.h"

struct arch_state {
    struct gdt_ctx gdt;
    struct idt_ctx idt;
};