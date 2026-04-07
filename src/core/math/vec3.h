#pragma once
#include <stdint.h>

#define $define_vec3(type, suffix) \
    typedef struct { \
        type x; \
        type y; \
        type z; \
    } vec2_##suffix##_t;

$define_vec3(uint32_t, uint32);
$define_vec3(float, fl);
$define_vec3(int, integer);

#undef $define_vec3

#define vec3(X, Y, Z) _Generic((X), \
    uint32_t: (vec2_uint32_t){ .x = (X), .y = (Y), z = (Z) }, \
    float:    (vec2_fl_t){ .x = (X), .y = (Y), z = (Z) }, \
    int:      (vec2_integer_t){ .x = (X), .y = (Y), z = (Z) } \
)