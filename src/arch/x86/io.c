#include "arch/io.h"

uint8_t ioread8(uint64_t addr) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"((uint16_t)addr));
    return ret;
}

void iowrite8(uint64_t addr, uint8_t data) {
    __asm__ volatile ("outb %0, %1" : : "a"(data), "Nd"((uint16_t)addr));
}

uint16_t ioread16(uint64_t addr) {
    uint16_t ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"((uint16_t)addr));
    return ret;
}

void iowrite16(uint64_t addr, uint16_t data) {
    __asm__ volatile ("outw %0, %1" : : "a"(data), "Nd"((uint16_t)addr));
}

uint32_t ioread32(uint64_t addr) {
    uint32_t ret;
    __asm__ volatile ("inl %1, %0" : "=a"(ret) : "Nd"((uint16_t)addr));
    return ret;
}

void iowrite32(uint64_t addr, uint32_t data) {
    __asm__ volatile ("outl %0, %1" : : "a"(data), "Nd"((uint16_t)addr));
}