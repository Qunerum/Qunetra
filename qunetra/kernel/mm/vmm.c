#include "../../utility/types.h"

static uint32 page_directory[1024] __attribute__((aligned(4096))),
first_page_table[1024] __attribute__((aligned(4096)));

void vmm_init(void) {
	for (uint16 i = 0; i < 1024; i++) first_page_table[i] = (i * 4096) | 3;
	page_directory[0] = ((uint32)(uiptr)first_page_table) | 3;
	for (uint16 i = 1; i < 1024; i++) page_directory[i] = 0;
	__asm__ volatile("mov %0, %%cr3" : : "r"(page_directory));
	uint32 cr0;
	__asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
	cr0 |= 0x80000000;
	__asm__ volatile("mov %0, %%cr0" : : "r"(cr0));
}
void vmm_map_page(const uint32 virtual_addr, const uint32 physical_addr, const uint32 flags) {
	const uint32 pt_index = (virtual_addr >> 12) & 0x03FF, pd_index = virtual_addr >> 22;
	if (pd_index == 0) {
		first_page_table[pt_index] = (physical_addr & ~0xFFF) | (flags & 0xFFF);
		__asm__ volatile("invlpg (%0)" : : "r"(virtual_addr) : "memory");
	}
}
