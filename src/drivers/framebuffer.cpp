#include "framebuffer.hpp"

void framebuffer::put_pixel(vec2<uint32_t> pos, uint32_t color) {
    if (pos.x >= fb->width || pos.y >= fb->height) {
        return;
    }

    uint32_t row_size_in_pixels = fb->pitch / 4;

    uint64_t offset = (pos.y * row_size_in_pixels) + pos.x;

    fb->addr[offset] = color;
}