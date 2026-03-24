#pragma once
#include <stddef.h>

typedef struct string {
    char data[256];
    size_t length;
    size_t capacity;
} string_t;

void str_append(string_t* str, const char* text);
void str_clear(string_t* str);
