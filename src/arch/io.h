#pragma once
#include <stdint.h>

uint8_t ioread8(uint32_t addr);
void iowrite8(uint32_t addr, uint8_t data);

uint32_t ioread32(uint32_t addr);
void iowrite32(uint32_t addr, uint32_t data);