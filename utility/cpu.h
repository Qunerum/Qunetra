#ifndef CPU_H
#define CPU_H

#include "types.h"

static inline uint8 inb(const uint16 port) {
	uint8 val;
	__asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
	return val;
}

void vga(const char *str);
void freeze();

#endif
