#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

enum arg_type_enum {
    ARG_INT,
    ARG_UINT,
    ARG_HEX,
    ARG_STR,
    ARG_PTR,
    ARG_CHAR,
};

typedef struct arg_type {
    enum arg_type_enum type;
    union {
        int64_t i;
        uint64_t u;
        void* p;
        char c;
    };
} arg_t;

void vstrfmt(char* buffer, size_t size, const char* fmt, va_list args);
void strfmt(char* buffer, size_t size, const char* fmt, ...);

void vformat(const char* fmt, arg_t args[], size_t arg_count);

#define _pack_arg(x) _Generic((x), \
    char:               (arg_t){ARG_CHAR, .c = (x)}, \
    int:                (arg_t){ARG_INT, .i = (x)}, \
    unsigned int:       (arg_t){ARG_UINT, .u = (x)}, \
    long long:          (arg_t){ARG_INT, .i = (x)}, \
    unsigned long long: (arg_t){ARG_UINT, .u = (x)}, \
    char*:              (arg_t){ARG_STR, .p = (x)}, \
    const char*:        (arg_t){ARG_STR, .p = (x)}, \
    default:            (arg_t){ARG_PTR, .p = (void*)(uintptr_t)(x)} \
)

#define _arg_count(...) (sizeof((arg_t[]){__VA_ARGS__}) / sizeof(arg_t))

#define _get_map(_1, _2, _3, _4, _5, _6, _7, _8, NAME, ...) NAME

#define _map1(m, x)      m(x)
#define _map2(m, x, ...) m(x), _map1(m, __VA_ARGS__)
#define _map3(m, x, ...) m(x), _map2(m, __VA_ARGS__)
#define _map4(m, x, ...) m(x), _map3(m, __VA_ARGS__)
#define _map5(m, x, ...) m(x), _map4(m, __VA_ARGS__)
#define _map6(m, x, ...) m(x), _map5(m, __VA_ARGS__)
#define _map7(m, x, ...) m(x), _map6(m, __VA_ARGS__)
#define _map8(m, x, ...) m(x), _map7(m, __VA_ARGS__)

#define _map(m, ...) \
    _get_map(__VA_ARGS__, _map8, _map7, _map6, _map5, _map4, _map3, _map2, _map1)(m, __VA_ARGS__)

#define format(fmt, ...) \
    vformat(fmt, \
        (arg_t[]){ __VA_OPT__(_map(_pack_arg, __VA_ARGS__)) }, \
        __VA_OPT__(_arg_count(_map(_pack_arg, __VA_ARGS__))) + 0 \
    )
