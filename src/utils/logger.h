#pragma once
#include <stdint.h>
#include "types/string.h"

// #define kprint(fmt, ...)\
// {\
//     char buf[2048] = {0};\
//     sprintf(buf, fmt, __VA_ARGS__);\

// }

// #define $log_debug(msg, ...)    kprint()
// #define $log_info(msg, ...)     kprint()
// #define $log_warning(msg, ...)  kprint()
// #define $log_error(msg, ...)    kprint()
// #define $log_critical(msg, ...) kprint()

typedef struct serial_ports {
    uint8_t qemu_debug_port;
} serial_ports_t;

constexpr serial_ports_t serial_ports = {
    .qemu_debug_port = 0xE9,
};

void qemu_print(string_t* str);