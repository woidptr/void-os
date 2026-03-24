#pragma once
#include <stddef.h>

typedef struct string {
    char* data;
    size_t length;
    size_t capacity;
} string_t;

string_t strnew(const char* text);
size_t strlen(const char* text);
void str_append(string_t* str, const char* text);
void str_clear(string_t* str);
