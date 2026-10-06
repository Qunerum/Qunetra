#ifndef PMM_H
#define PMM_H

#include "../../utility/types.h"

void pmm_init(uint32 mem_upper_kb);
void *pmm_alloc_block(void);
void pmm_free_block(void *addr);

#endif
