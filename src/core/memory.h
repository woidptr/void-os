#pragma once
#include <stddef.h>
#include "limine.h"
#include "runtime.h"

void memory_init(runtime_context_t* runtime_ctx);
void* malloc(size_t size);
void* memcpy(void* dest, const void* src, size_t count);
void memfree(void* ptr);