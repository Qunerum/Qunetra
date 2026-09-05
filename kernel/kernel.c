#include "../utility/string.h"
#include "../utility/memory.h"
#include "../utility/types.h"
#include "../utility/math.h"
#include "../utility/cpu.h"

#include "../drivers/lfb.h"
#include "../drivers/storage/ata.h"
#include "../drivers/storage/storage.h"

#include "console.h"

#define DISTANCE 35
static void prtState(const char* name, const state stat) {
	uint l = strLen(name);
	if (l >= DISTANCE) return;
	uint s =  DISTANCE - l - 6;
	kprintf("%q%s", 8, name);
	for (uint i = 0; i < s; i++) putChar(' ');
	kprintf("%q[%q%s%q]\n", 4, stat ? 132 : 20, stat ? " OK " : "FAIL", 4);
}
static uint8 getNumLen(uint v) {
	if (v == 0) return 1;
	uint length = 0;
	while (v > 0) {
		length++;
		v /= 10;
	}
	return length;
}
static void prtStateNxN(const char* name, const uint a, const uint b) {
	const state stat = a == b;
	const uint la = getNumLen(a), lb = getNumLen(b),
	l = strLen(name), m = DISTANCE - l - la - lb - 10;
	if (l >= m) return;
	kprintf("%q%s", 8, name);
	for (uint i = 0; i < m; i++) putChar(' ');
	kprintf("%q[%q%d%q/%q%d%q] [%q%s%q]\n", 4, 8, a, 4, 8, b, 4, stat ? 132 : 20, stat ? " OK " : "FAIL", 4);
}
static void prtCustom(const char* name, const char* custom) {
	const uint l = strLen(name), la = strLen(custom);
	if (l >= DISTANCE) return;
	const uint s =  DISTANCE - l - la - 2;
	kprintf("%q%s", 8, name);
	for (uint i = 0; i < s; i++) putChar(' ');
	kprintf("%q[%q%s%q]\n", 4, 8, custom, 4);
}
static void prtVal(const char* name, const uint v, const char* type) {
	const uint l = strLen(name), la = getNumLen(v), lb = strLen(type);
	if (l >= DISTANCE) return;
	const uint s =  DISTANCE - l - la - lb - 2;
	kprintf("%q%s", 8, name);
	for (uint i = 0; i < s; i++) putChar(' ');
	kprintf("%q[%q%d%s%q]\n", 4, 8, v, type, 4);
}
uint64 getHumanSize(const uint64 sectors, const char** memUnit) {
	if (sectors >= 2199023255552ULL) { *memUnit = " PiB"; return sectors / 2199023255552ULL; }
	else if (sectors >= 2147483648ULL) { *memUnit = " TiB"; return sectors / 2147483648ULL; }
	else if (sectors >= 2097152ULL) { *memUnit = " GiB"; return sectors / 2097152ULL; }
	else if (sectors >= 2048ULL) { *memUnit = " MiB"; return sectors / 2048ULL; }
	else if (sectors >= 2ULL) { *memUnit = " KiB"; return sectors / 2ULL; }
	else { *memUnit = " B"; return sectors * 512ULL; }
	return 0;
}

void closeQunetra() {
	closeStorage();
}
void kernel_main(const uint32 magic, const uint32 addr) {
	if (magic != 0x36d76289) {
		vga("ERROR: Bad Multiboot2 Magic Number!");
		return;
	}
	const state lfb = initLFB(addr);
	lfbFill(0);
	kprintf("%q=-=-=-=-=-=-= %qQunetra %q=-=-=-=-=-=-=\n", 4, 13, 4);
	prtState("Magic number", true);
	prtState("LFB", lfb);
	prtState("Kernel", true);
	kprintf("%q=-= %qStorage %q=-=\n", 4, 13, 4);
	prtCustom("Controller", initStorage());
	const char* memUnit;
	const uint64 sectors = ata_getSectorCount(), diskMem = getHumanSize(sectors, &memUnit);
	prtVal("Sectors", sectors, "");
	prtVal("Disk memory", diskMem, memUnit);
	kprintf("%q=-= %qUtility %q=-=\n", 4, 13, 4);
	prtStateNxN("string.h", stringTest(), 20);
	prtStateNxN("memory.h", memoryTest(), 3);
	prtStateNxN("math.h", mathTest() * 2, 10);
	putChar('\n');
	for (uint8 y = 0; y < 16; y++) {
		for (uint8 x = 0; x < 16; x++) { setCharColor(y * 16 + x); putChar('\x80'); putChar('\x80'); }
		putChar('\n');
	}
	while (1) {
		if (inb(0x64) & 1) {
			const uint8 scancode = inb(0x60);
			if (!(scancode & 0x80)) {
				// drawRect(200, 200, 50, 50, 0x00FF00);
			}
		}
	}
}
