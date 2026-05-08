#include "core/kernel.h"
#include "core/hal/cpu.h"
#include "drivers/framebuffer.hpp"
#include "runtime.hpp"

extern "C" void executive_main(struct kernel_ctx* kctx) {
    cpp_runtime_init();
    // framebuffer fb = framebuffer(kctx->framebuffer);

    // fb.fill(0xFFFFFFFF);

    cpu_lock();
}