#include "resources.h"
constexpr uint8_t asset_0[] = {
    #embed "ter-u16n.psf"
};
constexpr uint8_t asset_1[] = {
    #embed "ter-v16n.psf"
};

typedef struct embedded_resource {
    uint64_t path_hash;
    const uint8_t* data;
    size_t size;
} embedded_resource_t;

static const embedded_resource_t resources[2] = {
    { 8401067076303466178ULL, asset_0, sizeof(asset_0) },
    { 15005877801542842409ULL, asset_1, sizeof(asset_1) },
};

resource_t resource_get(uint64_t path_hash) {
    for (size_t i = 0; i < 2; i++) {
        if (resources[i].path_hash == path_hash) {
            return (resource_t){
                .data = resources[i].data,
                .size = resources[i].size,
            };
        }
    }

    return (resource_t){ .data = nullptr, .size = 0 };
}