#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "limine.h"
#include "cpu/idt.h"

__attribute__((used, section(".limine_requests")))
static volatile uint64_t requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests")))
static volatile uint64_t requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

__attribute__((used, section(".limine_requests")))
static volatile uint64_t base_revision[] = { LIMINE_BASE_REVISION(3) };

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

static void hlt() {
    for (;;) {
        __asm__ ("hlt");
    }
}

void kernel_main() {
    struct limine_framebuffer_response *fb_response = framebuffer_request.response;

    if (fb_response == NULL || fb_response->framebuffer_count < 1) {
        hlt();
    }

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