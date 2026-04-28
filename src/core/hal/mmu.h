#pragma once
#include <stdint.h>
#include <stddef.h>
#include "core/mm/pmm.h"

#define HAL_PAGE_PRESENT  (1 << 0)
#define HAL_PAGE_WRITABLE (1 << 1)
#define HAL_PAGE_USER     (1 << 2)

void* mmu_create_directory(struct pmm_ctx* pmm, uint64_t hhdm_offset);
void  mmu_map_page(void* directory, uint64_t virt, uint64_t phys, uint32_t flags, struct pmm_ctx* pmm, uint64_t hhdm_offset);
void  mmu_switch_directory(void* directory_phys);