#pragma once
#include "kernel.h"
#include "lib/math.hpp"

class framebuffer {
    struct framebuffer_ctx* fb;
public:
    framebuffer(struct framebuffer_ctx* fb) : fb(fb) { }

    void put_pixel(vec2<uint32_t> pos, uint32_t color);

    void fill(uint32_t color) {
        for (uint32_t y = 0; y < fb->height; y++) {
            for (uint32_t x = 0; x < fb->width; x++) {
                put_pixel({x, y}, color);
            }
        }
    }
};