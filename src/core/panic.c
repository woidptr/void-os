#include "panic.h"
#include "core/hal/interrupts.h"
#include "core/hal/cpu.h"

void kernel_panic() {
    interrupts_disable();

    cpu_lock();
}