#include "../utility/string.h"
#include "../utility/types.h"
#include "../utility/cpu.h"
#include "../drivers/lfb.h"
#include "console.h"

void prtState(const char* name, const state stat) {
	uint l = strLen(name);
	if (l >= 32) return;
	uint s =  32 - l;
	kprintf("%q%s", 8, name);
	for (uint i = 0; i < s; i++) putChar(' ');
	kprintf("%q[%q%s%q]\n", 4, stat ? 132 : 20, stat ? " OK " : "FAIL", 4);
}
void prtStateNxN(const char* name, const uint a, const uint b) {
	state stat = a == b;
	uint la = a >= 100 ? 3 : a >= 10 ? 2 : 1,
	lb = b >= 100 ? 3 : b >= 10 ? 2 : 1,
	l = strLen(name), m = 32 - 4 - la - lb;
	if (l >= m) return;
	uint s =  m - l;
	kprintf("%q%s", 8, name);
	for (uint i = 0; i < s; i++) putChar(' ');
	kprintf("%q[%q%d%q/%q%d%q] [%q%s%q]\n", 4, 8, a, 4, 8, b, 4, stat ? 132 : 20, stat ? " OK " : "FAIL", 4);
}

void kernel_main(const uint32 magic, const uint32 addr) {
	if (magic != 0x36d76289) {
		vga("ERROR: Bad Multiboot2 Magic Number!");
		return;
	}
	state lfb = initLFB(addr);
	lfbFill(0);
	prtState("Magic number", true);
	prtState("LFB", lfb);
	kprintf("%q=-= Utility =-=\n", 4);
	prtStateNxN("string.h", stringTest(), 16);
	putChar('\n');
	for (uint8 y = 0; y < 16; y++) {
		for (uint8 x = 0; x < 16; x++) { setCharColor(y * 16 + x); putChar('\x80'); }
		putChar('\n');
	}
	state state = false;
	while (1) {
		if (inb(0x64) & 1) {
			const uint8 scancode = inb(0x60);
			if (!(scancode & 0x80)) {
				state = !state;
				// drawRect(200, 200, 50, 50, state ? 0x00FF00 : 0xFF0000);
			}
		}
	}
}
