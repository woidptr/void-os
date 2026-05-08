#include "panic.h"
#include "core/hal/cpu.h"
#include "core/kernel.h"
#include "format.h"

void kernel_panic(struct kernel_ctx* kctx, const char* fmt, ...) {
    cpu_disbale_interrupts();

    if (kctx) {
        // fb_draw_text(&kctx->framebuffer, "KERNEL PANIC");

        char error_msg[256];
        va_list args;
        va_start(args, fmt);
        vstrfmt(error_msg, sizeof(error_msg), fmt, args);
        va_end(args);

        char error_buf[300];
        strfmt(error_buf, sizeof(error_buf), "REASON: %s", error_msg);
        // fb_draw_text(&kctx->framebuffer, error_buf);

        // fb_draw_text(&kctx->framebuffer, "CALL STACK TRACE:");

        struct stack_frame* frame = nullptr;

        if (true) {
            frame = nullptr;
        } else {
            frame = (struct stack_frame*)__builtin_frame_address(0);
        }

        int depth = 0;
        while (frame != nullptr && (uintptr_t)frame > 0x1000 && depth < 15) {
            char trace_line[128];
            strfmt(trace_line, sizeof(trace_line), "[%d] ADDR: %p\n", depth, frame->return_addr);
            // fb_draw_text(&kctx->framebuffer, trace_line);

            frame = frame->next_frame;
            depth++;
        }
    }

    cpu_lock();
}