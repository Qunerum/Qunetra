#include "types.h"

void vga(const char *str) {
	volatile uint16 *vga = (volatile uint16 *)0xB8000;
	for (int i = 0; str[i] != '\0'; i++) vga[i] = (uint16)str[i] | 0x4F00;
}

void freeze() { while (1) __asm__ volatile ("hlt"); }
