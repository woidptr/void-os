#pragma once
#include <stdint.h>
#include <stddef.h>

typedef struct resource {
    const uint8_t* data;
    size_t size;
} resource_t;

resource_t resource_get(uint64_t path_hash);