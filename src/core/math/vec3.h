#pragma once
#include <stdint.h>

#define DEFINE_VEC3(type, suffix) \
    typedef struct { \
        type x; \
        type y; \
        type z; \
    } vec2_##suffix##_t;

DEFINE_VEC3(uint32_t, uint32);
DEFINE_VEC3(float, fl)
DEFINE_VEC3(int, integer)

#undef DEFINE_VEC3

#define vec3(X, Y, Z) _Generic((X), \
    uint32_t: (vec2_uint32_t){ .x = (X), .y = (Y), z = (Z) }, \
    float:    (vec2_fl_t){ .x = (X), .y = (Y), z = (Z) }, \
    int:      (vec2_integer_t){ .x = (X), .y = (Y), z = (Z) } \
)