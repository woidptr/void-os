#pragma once
#include "core/graphics/gfx.h"
#include "core/kernel.h"

void softgfx_init(struct kernel_ctx* kctx, uint32_t* phys_fb, uint32_t width, uint32_t height);