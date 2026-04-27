#pragma once
#include "kernel.h"
#include "lib/math.hpp"

class framebuffer {
    struct framebuffer_ctx* fb;
public:
    framebuffer(struct framebuffer_ctx* fb) : fb(fb) { }

    void put_pixel(vec2<uint32_t> pos, uint32_t color);
};