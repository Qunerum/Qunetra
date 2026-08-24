#include "../utility/types.h"
#include "../utility/cpu.h"
#include "../drivers/lfb.h"

void kernel_main(const uint32 magic, const uint32 addr) {
	if (magic != 0x36d76289) {
		vga("ERROR: Bad Multiboot2 Magic Number!");
		return;
	}
	initLFB(addr);

	drawRect(0, 0, 100, 100, 0xFF0000);

	state state = 0;
	while (1) {
		if (inb(0x64) & 1) {
			const uint8 scancode = inb(0x60);
			if (!(scancode & 0x80)) {
				state = !state;
				drawRect(200, 200, 50, 50, state ? 0x00FF00 : 0xFF0000);
				scrollUpN(5);
			}
		}
	}
}
