#pragma once
#include <stdint.h>

typedef struct framebuffer_context {
    uint32_t* addr;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
} framebuffer_context_t;

typedef struct runtime_context {
    uint64_t hhdm_offset;
    framebuffer_context_t* framebuffer;
} runtime_context_t;