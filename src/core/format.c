#include "format.h"
#include "core/hal/serial.h"

static void push_char(char** buffer, size_t* size_left, char c) {
    if (*size_left > 1) {
        **buffer = c;
        (*buffer)++;
        (*size_left)--;
    }
}

static void push_int(char** buffer, size_t* size_left, uint64_t val, int base, int is_signed) {

}

void vstrfmt(char* buffer, size_t size, const char* fmt, va_list args) {
    if (!buffer || size == 0 || !fmt) return;

    char* ptr = buffer;
    size_t remain = size;

    while (*fmt != '\0') {
        if (*fmt == '%') {
            fmt++;
            if (fmt == '\0') break;

            switch (*fmt) {
                case 's': {
                    const char* str = va_arg(args, const char*);
                    if (!str) str = "(null)";
                    while (*str) push_char(&ptr, &remain, *str++);
                    break;
                }
                case 'c': {
                    push_char(&ptr, &remain, (char)va_arg(args, int));
                    break;
                }
                case 'd': {
                    [[fallthrough]];
                }
                case 'i': {
                    break;
                }
                case '%': {
                    push_char(&ptr, &remain, '%');
                    break;
                }
                default: {
                    push_char(&ptr, &remain, '%');
                    push_char(&ptr, &remain, *fmt);
                    break;
                }
            }
        } else {
            push_char(&ptr, &remain, *fmt);
        }
    }
}

void strfmt(char* buffer, size_t size, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vstrfmt(buffer, size, fmt, args);
    va_end(args);
}

void vformat(const char* fmt, arg_t args[], size_t arg_count) {
    size_t arg_ptr = 0;

    for (const char* p = fmt; *p != '\0'; p++) {
        if (*p == '{' && *(p + 1) == '}' && arg_ptr < arg_count) {
            arg_t* a = &args[arg_ptr++];

            switch (a->type) {
                case ARG_INT: {
                    // print_i(a->i);
                    break;
                }
                case ARG_UINT: {
                    // print_u(a->u);
                    break;
                }
            }
            p++;
            continue;
        }

        serial_putc(*p);
    }
}