#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "limine.h"
#include "cpu/idt.h"
#include "utils/logger.h"
#include "memory.h"
#include "runtime.h"

__attribute__((used, section(".limine_requests")))
static volatile uint64_t requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests")))
static volatile uint64_t base_revision[] = { LIMINE_BASE_REVISION(3) };

__attribute__((used, section(".limine_requests")))
volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST_ID,
    .revision = 0,
};

__attribute__((used, section(".limine_requests")))
volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST_ID,
    .revision = 0,
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile uint64_t requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

static void hlt() {
    for (;;) {
        __asm__ ("hlt");
    }
}

// typedef struct framebuffer_context {
//     void* addr;
//     uint32_t width;
//     uint32_t height;
// } framebuffer_context_t;

runtime_context_t init_limine() {
    struct limine_hhdm_response* hhdm = hhdm_request.response;

    runtime_context_t runtime_ctx = {
        .hhdm_offset = hhdm->offset,
    };

    return runtime_ctx;
}

void kernel_main() {
    struct limine_framebuffer_response *fb_response = framebuffer_request.response;
    
    if (fb_response == NULL || fb_response->framebuffer_count < 1) {
        hlt();
    }
    
    runtime_context_t runtime_ctx = init_limine();

    memory_init(&runtime_ctx);

    string_t str = strnew("Testing the new string implementation");
    qemu_print(&str);

    struct limine_framebuffer *fb = fb_response->framebuffers[0];
    uint32_t *fb_ptr = (uint32_t *)fb->address;

    if (fb_response != NULL) {
        struct limine_framebuffer *fb = fb_response->framebuffers[0];
        uint32_t *fb_ptr = (uint32_t *)fb->address;
        for (size_t i = 0; i < (fb->pitch / 4) * fb->height; i++) {
            fb_ptr[i] = 0xFFFFFFFF;
        }
    }

    idt_init();

    hlt();
}