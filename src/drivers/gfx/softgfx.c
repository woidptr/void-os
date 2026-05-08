#include "backends.h"
#include "core/mm/heap.h"
#include "lib/cmem.h"
#include "core/hal/cpu.h"

struct cpu_gfx_state {
    uint32_t* frontbuffer;
    uint32_t* backbuffer;
    float* zbuffer;
    uint32_t width;
    uint32_t height;
    gfx_matrix_t current_transform;
};

static void cpu_clear(gfx_driver_state_t opaque_state, uint32_t flags, uint32_t color, float depth) {
    struct cpu_gfx_state* state = (struct cpu_gfx_state*)opaque_state;
    uint32_t pixel_count = state->width * state->height;

    if (flags & GFX_CLEAR_COLOR) {
        for (uint32_t i = 0; i < pixel_count; i++) {
            state->backbuffer[i] = color;
        }
    }

    if ((flags & GFX_CLEAR_DEPTH) && state->zbuffer) {
        for (uint32_t i = 0; i < pixel_count; i++) {
            state->zbuffer[i] = depth;
        }
    }
}

static void transform_vertex(const gfx_matrix_t* matrix, const gfx_vertex_t* in, gfx_vertex_t* out) {
    float x = in->x;
    float y = in->y;
    float z = in->z;
    float w = 1.0f;

    float tx = x * matrix->m[0][0] + y * matrix->m[1][0] + z * matrix->m[2][0] + w * matrix->m[3][0];
    float ty = x * matrix->m[0][1] + y * matrix->m[1][1] + z * matrix->m[2][1] + w * matrix->m[3][1];
    float tz = x * matrix->m[0][2] + y * matrix->m[1][2] + z * matrix->m[2][2] + w * matrix->m[3][2];
    float tw = x * matrix->m[0][3] + y * matrix->m[1][3] + z * matrix->m[2][3] + w * matrix->m[3][3];

    if (tw != 0.0f) {
        tx /= tw;
        ty /= tw;
        tz /= tw;
    }

    out->x = tx;
    out->y = ty;
    out->z = tz;
    out->color = in->color;
}

static void put_pixel(struct cpu_gfx_state* state, int x, int y, float z, uint32_t color) {
    if (x < 0 || x >= (int)state->width || y < 0 || y >= (int)state->height) {
        return;
    }

    int index = y * state->width + x;

    if (z < state->zbuffer[index]) {
        state->zbuffer[index] = z;
        state->backbuffer[index] = color;
    }
}

static void cpu_draw_vertices(gfx_driver_state_t opaque_state, const gfx_vertex_t* vertices, uint32_t count, enum gfx_topology topology) {
    struct cpu_gfx_state* state = (struct cpu_gfx_state*)opaque_state;

    gfx_vertex_t screen_verts[count];

    for (uint32_t i = 0; i < count; i++) {
        transform_vertex(&state->current_transform, &vertices[i], &screen_verts[i]);

        screen_verts[i].x = (screen_verts[i].x + 1.0f) * 0.5f * state->width;
        screen_verts[i].y = (1.0f - screen_verts[i].y) * 0.5f * state->height;
    }

    if (topology == GFX_TOPOLOGY_POINTS) {
        for (uint32_t i = 0; i < count; i++) {
            put_pixel(state, (int)screen_verts[i].x, (int)screen_verts[i].y, screen_verts[i].z, screen_verts[i].color);
        }
    } else if (topology == GFX_TOPOLOGY_LINES) {

    }
}

static void cpu_swap_buffers(gfx_driver_state_t opaque_state) {
    struct cpu_gfx_state* state = (struct cpu_gfx_state*)opaque_state;

    size_t bytes = state->width * state->height * sizeof(uint32_t);

    memcpy(state->frontbuffer, state->backbuffer, bytes);
}

void softgfx_init(struct kernel_ctx* kctx, uint32_t* phys_fb, uint32_t width, uint32_t height) {
    struct cpu_gfx_state* state = (struct cpu_gfx_state*)heap_alloc(&kctx->heap, sizeof(struct cpu_gfx_state));

    state->frontbuffer = phys_fb;
    state->backbuffer = heap_alloc(&kctx->heap, width * height * sizeof(uint32_t));
    state->width = width;
    state->height = height;
    state->zbuffer = (float*)heap_alloc(&kctx->heap, sizeof(float) * width * height);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state->current_transform.m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }

    struct gfx_backend* backend = heap_alloc(&kctx->heap, sizeof(struct gfx_backend));

    backend->clear = cpu_clear;
    backend->draw_vertices = cpu_draw_vertices;
    backend->swap_buffers = cpu_swap_buffers;

    gfx_init(&kctx->gfx, backend, state, width, height);
}