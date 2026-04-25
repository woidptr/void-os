#include "cmem.h"

void* memcpy(void* dest, const void* src, size_t count) {
    uint64_t* d64 = (uint64_t*)dest;
    const uint64_t* s64 = (const uint64_t*)src;

    size_t blocks = count / 8;
    for (size_t i = 0; i < blocks; i++) {
        d64[i] = s64[i];
    }

    uint8_t* d = (uint8_t*)(d64 + blocks);
    const uint8_t* s = (const uint8_t*)(s64 + blocks);

    for (size_t i = 0; i < (count % 8); i++) {
        d[i] = s[i];
    }

    return dest;
}

void* memset(void* dest, int value, size_t count) {
    uint8_t* d = (uint8_t*)dest;
    for (size_t i = 0; i < count; i++) {
        d[i] = (uint8_t)value;
    }
    return dest;
}

void* memslide(void* dest, const void* src, size_t count) {
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;

    if (d == s) {
        return dest;
    }

    if (d < s) {
        for (size_t i = 0; i < count; i++) {
            d[i] = s[i];
        }
    } else {
        for (size_t i = count; i > 0; i--) {
            d[i - 1] = s[i - 1];
        }
    }

    return dest;
}