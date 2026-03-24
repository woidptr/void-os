#include "logger.h"
#include "cpu/io.h"

void qemu_print(string_t* str) {
    for (int i = 0; str->length; i++) {
        outb(serial_ports.qemu_debug_port, str->data[i]);
    }
}