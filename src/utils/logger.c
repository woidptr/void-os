// #include "logger.h"
// #include "cpu/io.h"
// #include <stdarg.h>

// void qemu_print(string_t* str) {
//     for (size_t i = 0; i < str->length; i++) {
//         outb(serial_ports.qemu_debug_port, str->data[i]);
//     }
// }

// // void kprintf(const char* fmt, ...) {
// //     va_list args;
// //     va_start(args, fmt);

// //     while (*fmt != '\0') {
// //         if (*fmt == '%') {
// //             fmt++;

// //             switch (*fmt) {
// //                 case 's': {
// //                     const char* s = va_arg(args, const char*);
// //                     qemu_print(s ? s : "(null)");
// //                     break;
// //                 }
// //                 default: {
// //                     qemu_print('%');
// //                     qemu_print(fmt);
// //                     break;
// //                 }
// //             }
// //         }
// //     }

// //     va_end(args);
// // }