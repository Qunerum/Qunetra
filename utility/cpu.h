#ifndef CPU_H
#define CPU_H

#include "types.h"

static inline uint8 inb(const uint16 port) {
	uint8 val;
	__asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
	return val;
}
static inline uint32 inl(uint16 port) {
	uint32 ret;
	__asm__ volatile("inl %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}
static inline void insw(uint16 port, void *addr, uint32 count) { __asm__ volatile("cld; rep insw" : "+D"(addr), "+c"(count) : "d"(port) : "memory"); }

static inline void outb(uint16 port, uint8 data) { __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(port)); }
static inline void outl(uint16 port, uint32 val) { __asm__ volatile("outl %0, %1" : : "a"(val), "Nd"(port)); }
static inline void outsw(uint16 port, const void *addr, uint32 count) { __asm__ volatile("cld; rep outsw" : "+S"(addr), "+c"(count) : "d"(port)); }

static inline void vga(const char *str) {
	volatile uint16 *vga = (volatile uint16 *)0xB8000;
	for (uint i = 0; str[i] != '\0'; i++) vga[i] = (uint16)str[i] | 0x4F00;
}

static inline void freeze() { while (1) __asm__ volatile ("hlt"); }

#endif
