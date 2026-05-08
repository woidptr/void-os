#include <stdint.h>
#include <stddef.h>
#include <new>

#include "core/kernel.h"

extern "C" void (*__init_array_start[])();
extern "C" void (*__init_array_end[])();

void cpp_runtime_init() {
    size_t count = __init_array_end - __init_array_start;
    for (size_t i = 0; i < count; i++) {
        __init_array_start[i]();
    }
}