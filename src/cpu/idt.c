// #include "idt.h"
// #include "io.h"
// #include <stdint.h>
// #include "ports.h"

// extern void isr_stub(void);
// extern void isr33(void);
// extern void idt_load(void* ptr);

// idt_entry_t idt[256];
// idt_ptr_t idt_p;

// void default_handler(void) {
//     for (;;) {
//         __asm__ volatile ("hlt");
//     }
// }

// void idt_handler(uint64_t interrupt_num) {
//     if (interrupt_num == 33) {
//         uint8_t scancode = inb(0x60);
        
//         (void)scancode; 
//     }

//     if (interrupt_num >= 32 && interrupt_num <= 47) {
//         if (interrupt_num >= 40) {
//             outb(hardware_ports.pic2_cmd, 0x20);
//         }
//         outb(hardware_ports.pic1_cmd, 0x20);
//     }
// }

// void pic_remap(void) {
//     uint8_t a1, a2;

//     a1 = inb(hardware_ports.pic1_data);
//     a2 = inb(hardware_ports.pic2_data);

//     outb(hardware_ports.pic1_cmd, 0x11);
//     outb(hardware_ports.pic2_cmd, 0x11);

//     outb(hardware_ports.pic1_data, 0x20);
//     outb(hardware_ports.pic2_data, 0x28);

//     outb(hardware_ports.pic1_data, 4);
//     outb(hardware_ports.pic2_data, 2);

//     outb(hardware_ports.pic1_data, 0x01);
//     outb(hardware_ports.pic2_data, 0x01);

//     outb(hardware_ports.pic1_data, a1);
//     outb(hardware_ports.pic2_data, a2);
// }

// void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags) {
//     idt[num].offset_low  = (base & 0xFFFF);
//     idt[num].offset_mid  = (base >> 16) & 0xFFFF;
//     idt[num].offset_high = (base >> 32) & 0xFFFFFFFF;
//     idt[num].selector    = sel;
//     idt[num].ist         = 0;
//     idt[num].flags       = flags;
//     idt[num].reserved    = 0;
// }

// void idt_init(void) {
//     idt_p.limit = (sizeof(idt_entry_t) * 256) - 1;
//     idt_p.base = (uint64_t)&idt;

//     for (int i = 0; i < 256; i++) {
//         idt_set_gate(i, (uint64_t)isr_stub, 0x28, 0x8E);
//     }

//     idt_set_gate(33, (uint64_t)isr33, 0x28, 0x8E);

//     idt_load(&idt_p);

//     pic_remap();

//     outb(hardware_ports.pic1_data, 0xFD);
//     outb(hardware_ports.pic2_data, 0xFF);

//     __asm__ volatile ("sti");
// }