#pragma once
#include <stdint.h>

#define vec2_types(M) \
    M(uint32_t) \
    M(float) \
    M(int) \
    M(double)

#define generate_vec2_struct(type) \
    typedef struct { type x; type y; } vec2_##type;

#define generate_vec2_init(type) \
    static inline vec2_##type vec2_##type##_init(type x, type y) { \
        return (vec2_##type){ .x = x, .y = y }; \
    }

#define generate_vec2_eq(type) \
    static inline bool vec2_##type##_eq(vec2_##type a, vec2_##type b) { \
        return (a.x == b.x) && (a.y == b.y); \
    }

vec2_types(generate_vec2_struct);
vec2_types(generate_vec2_init);
vec2_types(generate_vec2_eq);

#define assoc_vec2_init(type) type: vec2_##type##_init,
#define assoc_vec2_eq(type) type: vec2_##type##_init,

#define vec2(X, Y) _Generic((X), \
    vec2_types(assoc_vec2_init) \
    default: vec2_int_init \
)((X), (Y))

#define vec2_eq(A, B) _Generic((X), \
    vec2_types(assoc_vec2_eq) \
    default: vec2_int_eq \
)((X), (Y))

// #define vec2_add(A, B) _Generic((A), \
//     vec2_uint32_t:  vec2_uint32_add, \
//     vec2_fl_t:      vec2_fl_add, \
//     vec2_integer_t: vec2_integer_add \
// )

// #define vec2_eq(A, B) _Generic((A), \
//     vec2_uint32_t:  vec2_uint32_eq, \
//     vec2_fl_t:      vec2_fl_eq, \
//     vec2_integer_t: vec2_integer_eq \
// )