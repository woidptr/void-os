#include "core/hal/cpu.h"

void cpu_halt() {
    __asm__ volatile ("hlt");
}