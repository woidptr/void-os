#pragma once
#include <stdint.h>

static constexpr uint32_t psf2_magic = 0x864ab572;

typedef struct psf2_header {
    uint32_t magic;
    uint32_t version;
    uint32_t headersize;
    uint32_t flags;
    uint32_t numglyph;
    uint32_t bytesperglyph;
    uint32_t height;
    uint32_t width;
} psf2_header_t;

extern psf2_header_t* active_font;