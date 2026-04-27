#pragma once
#include "arch/x86/cpu/gdt.h"

struct arch_state {
    struct gdt_ctx gdt;
};