#pragma once
#include <stddef.h>

typedef struct string {
    char* data;
    size_t length;
    size_t capacity;
} string_t;

typedef union __attribute__((transparent_union)) {
    char* raw;
    string_t* obj;
} string_arg_t;

string_t strnew(const char* text);
void strfree(string_t* str);
void strappend(string_t* str, const char* text);
void strclear(string_t* str);