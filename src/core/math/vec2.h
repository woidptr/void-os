#pragma once
#include <stdint.h>

#define $define_vec2(type, suffix) \
    typedef struct { \
        type x; \
        type y; \
    } vec2_##suffix##_t;

$define_vec2(uint32_t, uint32);
$define_vec2(float, fl)
$define_vec2(int, integer)

#undef $define_vec2

#define $vec2_new(X, Y) _Generic((X), \
    uint32_t: (vec2_uint32_t){ .x = (X), .y = (Y) }, \
    float:    (vec2_fl_t){ .x = (X), .y = (Y) }, \
    int:      (vec2_integer_t){ .x = (X), .y = (Y) } \
)