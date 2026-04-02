#include "graphics.h"

// static constexpr uint8_t terminus_font[] = {
//     #embed "ter-v16n.psf"
// };

void graphics_init(runtime_context_t* runtime_ctx) {

}

void put_pixel(runtime_context_t* runtime_ctx, vec2_uint32_t pos, uint32_t color) {
    if (pos.x >= runtime_ctx->framebuffer->width || pos.y >= runtime_ctx->framebuffer->height) {
        return;
    }

    uint32_t row_size_in_pixels = runtime_ctx->framebuffer->pitch / 4;

    uint64_t offset = (pos.y * row_size_in_pixels) + pos.y;

    runtime_ctx->framebuffer->addr[offset] = color;
}

static const uint8_t tiny_font[2][16] = {
    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },

    { 0x00, 0x00, 0x18, 0x3C, 0x66, 0x66, 0x7E, 0x66, 
      0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00 }
};

static uint32_t cursor_x = 0;
static uint32_t cursor_y = 0;

void draw_char(runtime_context_t* runtime_ctx, char c, uint32_t fg_color, uint32_t bg_color) {
    int glyph_index = (c == 'A' || c == 'a') ? 1 : 0;
    const uint8_t* glyph = tiny_font[glyph_index];

    for (uint32_t row = 0; row < 16; row++) {
        for (uint32_t col = 0; col < 8; col++) {
            
            vec2_uint32_t pos = vec2(cursor_x + col, cursor_y + row);
            if (glyph[row] & (1 << (7 - col))) {
                put_pixel(runtime_ctx, pos, fg_color);
            } else {
                put_pixel(runtime_ctx, pos, bg_color);
            }
        }
    }

    cursor_x += 8;
}

void draw_string(runtime_context_t* runtime_ctx, const char* text) {
    uint32_t i = 0;
    while (text[i] != '\0') {
        if (text[i] == '\n') {
            cursor_x = 0;
            cursor_y += 16;
        } else {
            draw_char(runtime_ctx, text[i], 0xFFFFFFFF, 0x00000000);
        }
        i++;
    }
}