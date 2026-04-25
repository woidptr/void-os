#pragma once
#include <stdint.h>
#include <stddef.h>
#include "lib/memory.hpp"

class string : public heap_allocated<string> {
private:
    friend class heap_allocated<string>;

    char* buffer;
    size_t length;
    size_t capacity;

    string(struct heap_ctx* heap, const char* text);
    ~string();
public:
    const char* c_str();
};