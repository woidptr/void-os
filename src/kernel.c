#include <stdint.h>

typedef struct kernel_ctx {
    uint16_t* terminal_buffer;
} kernel_ctx_t;

void clean_stdout(kernel_ctx_t* kernel_ctx) {
    
}

void print(kernel_ctx_t* kernel_ctx, const char* str) {
    int index = 0;

    while (str[index] != '\0') {
        uint16_t colored_char = (uint16_t)str[index] | (uint16_t)0x0F00;
        kernel_ctx->terminal_buffer[index] = colored_char;
        index++;
    }
}

void kmain() {
    kernel_ctx_t kernel_ctx = {
        .terminal_buffer = (uint16_t*)0xB8000,
    };

    print(&kernel_ctx, "Hello, World");
}