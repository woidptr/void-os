// #pragma once
// #include <stdint.h>

// typedef struct hardware_ports {
//     uint16_t pic1_cmd;
//     uint16_t pic1_data;
//     uint16_t pic2_cmd;
//     uint16_t pic2_data;

//     uint16_t ps2_data;
//     uint16_t ps2_cmd;

//     uint16_t pci_addr;
//     uint16_t pci_data;
// } hardware_ports_t;

// constexpr hardware_ports_t hardware_ports = {
//     .pic1_cmd = 0x20,
//     .pic1_data = 0x21,
//     .pic2_cmd = 0xA0,
//     .pic2_data = 0xA1,

//     .ps2_data = 0x60,
//     .ps2_cmd = 0x64,

//     .pci_addr = 0xCF8,
//     .pci_data = 0xCFC,
// };