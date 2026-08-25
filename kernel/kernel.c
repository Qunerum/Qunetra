#include "../utility/types.h"
#include "../utility/cpu.h"
#include "../drivers/lfb.h"
#include "console.h"

void prtState(const char* name, state stat) {
	kprintf("%q%s %q[%q%s%q]\n", 8, name, 4, stat ? 132 : 20, stat ? " OK " : "FAIL", 4);
}

void kernel_main(const uint32 magic, const uint32 addr) {
	if (magic != 0x36d76289) {
		vga("ERROR: Bad Multiboot2 Magic Number!");
		return;
	}
	lfbFill(0);

	prtState("LFB", initLFB(addr));

	kprintf("%qHello%q, %qWorld%q!\n\n", 128, 15, 132, 15);
	for (uint8 y = 0; y < 16; y++) {
		for (uint8 x = 0; x < 16; x++) kprintf("%q\x80", y * 16 + x);
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
