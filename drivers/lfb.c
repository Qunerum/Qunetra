#include "../utility/types.h"
#include "../utility/cpu.h"

struct multiboot_tag { uint32 type, size; } __attribute__((packed));
struct multiboot_tag_framebuffer {
	uint32 type, size;
	uint64 framebuffer_addr;
	uint32 framebuffer_pitch, framebuffer_width, framebuffer_height;
	uint8  framebuffer_bpp, framebuffer_type;
	uint16 reserved;
} __attribute__((packed));

static uint32 _width = 0, _height = 0, _pitch = 0, _bpp = 0, *_addr = 0;;

void initLFB(const uint32 address) {
	struct multiboot_tag *tag;
	for (tag = (struct multiboot_tag *) (uiptr) (address + 8); tag->type != 0; tag = (struct multiboot_tag *) ((uint8 *) tag + ((tag->size + 7) & ~7))) {
		if (tag->type == 8) {
			struct multiboot_tag_framebuffer *fb = (struct multiboot_tag_framebuffer *) tag;
			_width = fb->framebuffer_width;
			_height = fb->framebuffer_height;
			_pitch = fb->framebuffer_pitch;
			_bpp = fb->framebuffer_bpp;
			_addr = (uint32 *) (uiptr) fb->framebuffer_addr;
			break;
		}
	}
	if (_addr == 0) {
		vga("ERROR: LFB! Check grub.cfg");
		freeze();
	}
	if (_bpp != 32) {
		vga("ERROR: BPP mode not supported! Expected 32 bit.");
		freeze();
	}
}
void scrollUp() {
	for (uint32 y = 0; y < _height - 1; y++) {
		uint32 *dest_row = (uint32 *)((uint8 *)_addr + y * _pitch), *src_row  = (uint32 *)((uint8 *)_addr + (y + 1) * _pitch);
		for (uint32 x = 0; x < _width; x++) dest_row[x] = src_row[x];
	}
	uint32 *last_row = (uint32 *)((uint8 *)_addr + (_height - 1) * _pitch);
	for (uint32 x = 0; x < _width; x++) last_row[x] = 0;
}
void scrollUpN(uint32 n) { for (uint32 i = 0; i < n; i++) scrollUp(); }
void putPx(const uint32 x, const uint32 y, const uint32 color) {
	if (x >= _width || y >= _height) return;
	*(uint32 *)((uint8 *)_addr + y * _pitch + (x << 2)) = color;
}
void drawRect(const uint32 x, const uint32 y, const uint32 width, const uint32 height, const uint32 color) {
	if (x + width > _width || y + height > _height) return;
	for (uint32 row = 0; row < height; row++) {
		uint32 *pixel = (uint32 *)((uint8_t *)_addr + (y + row) * _pitch) + x;
		for (uint32 col = 0; col < width; col++) pixel[col] = color;
	}
}
void lfbClear(const uint32 color) {
	const uint32 total_pixels = _width * _height;
	if (_pitch == _width * 4) {
		uint32 *dest = _addr;
		for (uint32 i = 0; i < total_pixels; i++) dest[i] = color;
	} else drawRect(0, 0, _width, _height, color);
}
