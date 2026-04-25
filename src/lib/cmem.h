#pragma once
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* memcpy(void* dest, const void* src, size_t count);
void* memset(void* dest, int value, size_t count);
void* memslide(void* dest, const void* src, size_t count);

#ifdef __cplusplus
}
#endif