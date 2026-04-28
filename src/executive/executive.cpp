#include "kernel.h"
#include "core/hal/cpu.h"

#include "drivers/framebuffer.hpp"

extern "C" void executive_main(struct kernel_ctx* kctx) {
    framebuffer fb = framebuffer(kctx->framebuffer);

    fb.fill(0xFFFFFFFF);

    cpu_lock();
}