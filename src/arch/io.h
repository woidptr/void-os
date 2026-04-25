#pragma once
#include <stdint.h>

uint8_t  ioread8(uint64_t addr);
void     iowrite8(uint64_t addr, uint8_t data);

uint16_t ioread16(uint64_t addr);
void     iowrite16(uint64_t addr, uint16_t data);

uint32_t ioread32(uint64_t addr);
void     iowrite32(uint64_t addr, uint32_t data);

uint64_t ioread64(uint64_t addr);
void     iowrite64(uint64_t addr, uint64_t data);