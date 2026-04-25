// #include "graphics.h"
// #include "core/font.h"
// #include "utils/logger.h"
// #include "core/resources.h"

// psf2_header_t* active_font = nullptr;

// void graphics_init(struct kernel_ctx* runtime_ctx) {
//     uintptr_t font_hash = cstrhash("ter-u16n.psf");

//     resource_t font_res = resource_get(font_hash);

//     if (font_res.data == nullptr) {
//         string_t str = strnew("Failed to initialize font!");
//         qemu_print(&str);
//         strfree(&str);

//         return;
//     }

//     active_font = (psf2_header_t*)font_res.data;

//     if (!active_font || active_font->magic != psf2_magic) {
//         string_t str = strnew("Failed to initialize font!");
//         qemu_print(&str);
//         strfree(&str);

//         return;
//     }
// }

// void put_pixel(struct kernel_ctx* runtime_ctx, vec2_uint32_t pos, uint32_t color) {
//     if (pos.x >= runtime_ctx->framebuffer->width || pos.y >= runtime_ctx->framebuffer->height) {
//         return;
//     }

//     uint32_t row_size_in_pixels = runtime_ctx->framebuffer->pitch / 4;

//     uint64_t offset = (pos.y * row_size_in_pixels) + pos.x;

//     runtime_ctx->framebuffer->addr[offset] = color;
// }

// static uint32_t cursor_x = 0;
// static uint32_t cursor_y = 0;

// void draw_char(struct kernel_ctx* runtime_ctx, char c, uint32_t fg_color, uint32_t bg_color, uint32_t scale) {
//     uint32_t glyph_index = (uint8_t)c;

//     if (glyph_index >= active_font->numglyph) {
//         glyph_index = 0; 
//     }

//     uint8_t* font_data = (uint8_t*)active_font;
//     uint8_t* glyph = font_data + active_font->headersize + (glyph_index * active_font->bytesperglyph);

//     uint32_t bytes_per_row = (active_font->width + 7) / 8;

//     for (uint32_t row = 0; row < active_font->height; row++) {
//         for (uint32_t col = 0; col < active_font->width; col++) {
            
//             vec2_uint32_t pos = vec2(cursor_x + col, cursor_y + row);

//             uint32_t byte_offset = (row * bytes_per_row) + (col / 8);
//             uint32_t bit_index = 7 - (col % 8);

//             uint32_t current_color = (glyph[byte_offset] & (1 << bit_index)) ? fg_color : bg_color;

//             for (uint32_t sy = 0; sy < scale; sy++) {
//                 for (uint32_t sx = 0; sx < scale; sx++) {
//                     vec2_uint32_t pos = vec2(
//                         cursor_x + (col * scale) + sx, 
//                         cursor_y + (row * scale) + sy
//                     );
//                     put_pixel(runtime_ctx, pos, current_color);
//                 }
//             }
//         }
//     }

//     cursor_x += active_font->width * scale;
// }

// void draw_string(struct kernel_ctx* runtime_ctx, const char* text) {
//     uint32_t i = 0;
//     while (text[i] != '\0') {
//         if (text[i] == '\n') {
//             cursor_x = 0;
//             cursor_y += 16;
//         } else {
//             draw_char(runtime_ctx, text[i], 0xFFFFFFFF, 0x00000000, 1);
//         }
//         i++;
//     }
// }