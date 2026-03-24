#pragma once
#include <stddef.h>
#include "limine.h"

void memory_init();
void* malloc(size_t size);
void* memcpy(void* dest, const void* src, size_t count);