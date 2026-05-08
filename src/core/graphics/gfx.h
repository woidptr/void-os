#pragma once
#include <stdint.h>
#include <stddef.h>

typedef enum gfx_topology {
    GFX_TOPOLOGY_TRIANGLES,
    GFX_TOPOLOGY_LINES,
    GFX_TOPOLOGY_POINTS,
} gfx_topology_t;

typedef enum gfx_clear_flags {
    GFX_CLEAR_COLOR = (1 << 0),
    GFX_CLEAR_DEPTH = (1 << 1),
} gfx_clear_flags_t;

typedef struct {
    float x, y, z;
    uint32_t color;
    float u, v;
} gfx_vertex_t;

typedef struct {
    float m[4][4];
} gfx_matrix_t;

typedef void* gfx_driver_state_t;

struct gfx_backend {
    void (*clear)(gfx_driver_state_t state, uint32_t flags, uint32_t color, float depth);
    void (*draw_vertices)(gfx_driver_state_t state, const gfx_vertex_t* vertices, uint32_t count, enum gfx_topology topology);
    void (*swap_buffers)(gfx_driver_state_t state);
};

struct gfx_ctx {
    struct gfx_backend* backend;
    gfx_driver_state_t driver_state;

    uint32_t width;
    uint32_t height;
};

void gfx_init(struct gfx_ctx* gfx, struct gfx_backend* backend, gfx_driver_state_t state, uint32_t width, uint32_t height);
void gfx_clear(struct gfx_ctx* gfx, uint32_t flags, uint32_t color, float depth);
void gfx_draw_vertices(struct gfx_ctx* gfx, const gfx_vertex_t* vertices, uint32_t count, gfx_topology_t topology);
void gfx_swap_buffers(struct gfx_ctx* gfx);