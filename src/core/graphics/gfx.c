#include "gfx.h"

void gfx_init(struct gfx_ctx* gfx, struct gfx_backend* backend, gfx_driver_state_t state, uint32_t width, uint32_t height) {
    gfx->backend = backend;
    gfx->driver_state = state;
    gfx->width = width;
    gfx->height = height;
}

void gfx_clear(struct gfx_ctx* gfx, uint32_t flags, uint32_t color, float depth) {
    if (gfx && gfx->backend && gfx->backend->clear) {
        gfx->backend->clear(gfx->driver_state, flags, color, depth);
    }
}

void gfx_draw_vertices(struct gfx_ctx* gfx, const gfx_vertex_t* vertices, uint32_t count, gfx_topology_t topology) {
    if (gfx && gfx->backend && gfx->backend->draw_vertices) {
        gfx->backend->draw_vertices(gfx->driver_state, vertices, count, topology);
    }
}

void gfx_swap_buffers(struct gfx_ctx* gfx) {
    if (gfx && gfx->backend && gfx->backend->swap_buffers) {
        gfx->backend->swap_buffers(gfx->driver_state);
    }
}