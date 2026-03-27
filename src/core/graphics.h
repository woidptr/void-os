#pragma once
#include <stdint.h>
#include "runtime.h"
#include "core/math/vec2.h"

void put_pixel(runtime_context_t* runtime_ctx, vec2_uint32_t pos, uint32_t color);
void draw_char(runtime_context_t* runtime_ctx, char c, uint32_t fg_color, uint32_t bg_color);
void draw_string(runtime_context_t* runtime_ctx, const char* text);