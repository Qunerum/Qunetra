#ifndef VMM_H
#define VMM_H

#include "../../utility/types.h"

void vmm_init(void);
void vmm_map_page(uint32 virtual_addr, uint32 physical_addr, uint32 flags);

#endif
