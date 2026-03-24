#include "logger.h"
#include "cpu/io.h"

void qemu_print(string_t* str) {
    for (size_t i = 0; i < str->length; i++) {
        outb(serial_ports.qemu_debug_port, str->data[i]);
    }
}