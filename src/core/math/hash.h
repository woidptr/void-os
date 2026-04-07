#pragma once
#include <stdint.h>

typedef struct {
    uint64_t fnv_offset;
    uint64_t fnv_prime;
} fnv1a_hash_params_t;

static constexpr fnv1a_hash_params_t fnv1a_hash_params = {
    .fnv_offset = 0xCBF29CE484222325,
    .fnv_prime  = 0x100000001B3,
};

typedef struct hashmap {
    uint64_t hash;
} hashmap_t;
