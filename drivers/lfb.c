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
	for (tag = (struct multiboot_tag *) (uintptr_t) (address + 8); tag->type != 0; tag = (struct multiboot_tag *) ((uint8_t *) tag + ((tag->size + 7) & ~7))) {
		if (tag->type == 8) {
			struct multiboot_tag_framebuffer *fb = (struct multiboot_tag_framebuffer *) tag;
			_width = fb->framebuffer_width;
			_height = fb->framebuffer_height;
			_pitch = fb->framebuffer_pitch;
			_bpp = fb->framebuffer_bpp;
			_addr = (uint32_t *) (uintptr_t) fb->framebuffer_addr;
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
void putPx(const uint32_t x, const uint32_t y, const uint32_t color) {
	if (x >= _width || y >= _height) return;
	*(uint32_t *)((uint8_t *)_addr + y * _pitch + (x << 2)) = color;
}
void drawRect(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height, const uint32_t color) {
	if (x + width > _width || y + height > _height) return;
	for (uint32_t row = 0; row < height; row++) {
		uint32_t *pixel = (uint32_t *)((uint8_t *)_addr + (y + row) * _pitch) + x;
		for (uint32_t col = 0; col < width; col++) pixel[col] = color;
	}
}
void lfbClear(const uint32_t color) {
	const uint32_t total_pixels = _width * _height;
	if (_pitch == _width * 4) {
		uint32_t *dest = _addr;
		for (uint32_t i = 0; i < total_pixels; i++) dest[i] = color;
	} else drawRect(0, 0, _width, _height, color);
}
