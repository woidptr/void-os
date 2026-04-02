#pragma once
#include <stdint.h>

#define DEFINE_VEC2(type, suffix) \
    typedef struct { \
        type x; \
        type y; \
    } vec2_##suffix##_t; \
                         \
    static inline vec2_##suffix##_t vec2_##suffix##_add(vec2_##suffix##_t a, vec2_##suffix##_t b) { \
        return (vec2_##suffix##_t){ .x = a.x + b.x, .y = a.y + b.y }; \
    } \
      \
    static inline bool vec2_##suffix##_eq(vec2_##suffix##_t a, vec2_##suffix##_t b) { \
        return (a.x == b.x) && (a.y == b.y); \
    }

DEFINE_VEC2(uint32_t, uint32);
DEFINE_VEC2(float, fl);
DEFINE_VEC2(int, integer);

#undef DEFINE_VEC2

#define vec2(X, Y) _Generic((X), \
    uint32_t: (vec2_uint32_t){ .x = (X), .y = (Y) }, \
    float:    (vec2_fl_t){ .x = (X), .y = (Y) }, \
    int:      (vec2_integer_t){ .x = (X), .y = (Y) } \
)

#define vec2_add(A, B) _Generic((A), \
    vec2_uint32_t:  vec2_uint32_add, \
    vec2_fl_t:      vec2_fl_add, \
    vec2_integer_t: vec2_integer_add \
)

#define vec2_eq(A, B) _Generic((A), \
    vec2_uint32_t:  vec2_uint32_eq, \
    vec2_fl_t:      vec2_fl_eq, \
    vec2_integer_t: vec2_integer_eq \
)