#include "../utility/string.h"
#include "../utility/memory.h"
#include "../utility/types.h"
#include "../utility/cpu.h"

#include "../drivers/lfb.h"
#include "../drivers/storage/ata.h"
#include "../drivers/storage/storage.h"

#include "console.h"

#define DISTANCE 32
void prtState(const char* name, const state stat) {
	uint l = strLen(name);
	if (l >= DISTANCE) return;
	uint s =  DISTANCE - l;
	kprintf("%q%s", 8, name);
	for (uint i = 0; i < s; i++) putChar(' ');
	kprintf("%q[%q%s%q]\n", 4, stat ? 132 : 20, stat ? " OK " : "FAIL", 4);
}
void prtStateNxN(const char* name, const uint a, const uint b) {
	state stat = a == b;
	uint la = a >= 100 ? 3 : a >= 10 ? 2 : 1,
	lb = b >= 100 ? 3 : b >= 10 ? 2 : 1,
	l = strLen(name), m = DISTANCE - 4 - la - lb;
	if (l >= m) return;
	uint s =  m - l;
	kprintf("%q%s", 8, name);
	for (uint i = 0; i < s; i++) putChar(' ');
	kprintf("%q[%q%d%q/%q%d%q] [%q%s%q]\n", 4, 8, a, 4, 8, b, 4, stat ? 132 : 20, stat ? " OK " : "FAIL", 4);
}
void prtCustom(const char* name, const char* custom) {
	uint l = strLen(name), la = strLen(custom);
	if (l >= DISTANCE) return;
	uint s =  DISTANCE + 4 - l - la;
	kprintf("%q%s", 8, name);
	for (uint i = 0; i < s; i++) putChar(' ');
	kprintf("%q[%q%s%q]\n", 4, 8, custom, 4);
}
void test_disk_rw() {
	uint8 *write_buf = kmalloc(512), *read_buf = kmalloc(512);
	for (uint i = 0; i < 512; i++) {
		write_buf[i] = 0;
		read_buf[i] = 0;
	}
	const char *msg = "Qunetra: Write successful! Disk is working.";
	uint idx = 0;
	while (msg[idx] != '\0' && idx < 511) {
		write_buf[idx] = (uint8)msg[idx];
		idx++;
	}
	write_buf[idx] = '\0';

	uint32 sector = 1;

	kprintf("Writing on sector %d...\n", sector);
	ata_writeSector(sector, write_buf);

	kprintf("Reading from sector %d...\n", sector);
	ata_readSector(sector, read_buf);

	kprintf("Readed data: \n\n%s\n", (char*)read_buf);
}
void kernel_main(const uint32 magic, const uint32 addr) {
	if (magic != 0x36d76289) {
		vga("ERROR: Bad Multiboot2 Magic Number!");
		return;
	}
	state lfb = initLFB(addr);
	lfbFill(0);
	kprintf("%q=-=-=-=-=-=-= %qQunetra %q=-=-=-=-=-=-=\n", 4, 13, 4);
	prtState("Magic number", true);
	prtState("LFB", lfb);
	prtCustom("Storage controller", getStorageType());
	kprintf("%q=-= %qUtility %q=-=\n", 4, 13, 4);
	prtStateNxN("string.h", stringTest(), 16);
	prtStateNxN("memory.h", memoryTest(), 3);
	putChar('\n');
	for (uint8 y = 0; y < 16; y++) {
		for (uint8 x = 0; x < 16; x++) { setCharColor(y * 16 + x); putChar('\x80'); putChar('\x80'); }
		putChar('\n');
	}
	// test_disk_rw();
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
