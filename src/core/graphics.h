#pragma once
#include <stdint.h>
#include "core/math/vec2.h"

struct framebuffer_ctx {
    uint32_t* addr;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
};

// void graphics_init(struct kernel_ctx* runtime_ctx);
// void put_pixel(struct kernel_ctx* runtime_ctx, vec2_uint32_t pos, uint32_t color);
// void draw_char(struct kernel_ctx* runtime_ctx, char c, uint32_t fg_color, uint32_t bg_color, uint32_t scale);
// void draw_string(struct kernel_ctx* runtime_ctx, const char* text);