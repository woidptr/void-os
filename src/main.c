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

runtime_context_t init_limine() {
    runtime_context_t runtime_ctx = {0};
    static framebuffer_context_t framebuffer_ctx = {0};

    struct limine_hhdm_response* hhdm = hhdm_request.response;
    struct limine_framebuffer_response *fb_response = framebuffer_request.response;

    if (hhdm != nullptr) {
        runtime_ctx.hhdm_offset = hhdm->offset;
    }
    
    if (fb_response != NULL && fb_response->framebuffer_count > 0) {
        struct limine_framebuffer* fb = fb_response->framebuffers[0];

        framebuffer_ctx.addr = fb->address;
        framebuffer_ctx.width = fb->width;
        framebuffer_ctx.height = fb->height;
        framebuffer_ctx.pitch = fb->pitch;

        runtime_ctx.framebuffer = &framebuffer_ctx;
    }

    return runtime_ctx;
}

void kernel_main() {
    runtime_context_t runtime_ctx = init_limine();

    memory_init(&runtime_ctx);

    string_t str = strnew("Testing the new string implementation");
    qemu_print(&str);
    strfree(&str);

    for (size_t i = 0; i < (runtime_ctx.framebuffer->pitch / 4) * runtime_ctx.framebuffer->height; i++) {
        runtime_ctx.framebuffer->addr[i] = 0xFFFFFFFF;
    }

    idt_init();

    hlt();
}