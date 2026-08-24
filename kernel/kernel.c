#include <stdint.h>

struct multiboot_tag {
	uint32_t type;
	uint32_t size;
} __attribute__((packed));

struct multiboot_tag_framebuffer {
	uint32_t type;
	uint32_t size;
	uint64_t framebuffer_addr;
	uint32_t framebuffer_pitch;
	uint32_t framebuffer_width;
	uint32_t framebuffer_height;
	uint8_t  framebuffer_bpp;
	uint8_t  framebuffer_type;
	uint16_t reserved;
} __attribute__((packed));

uint32_t screen_width = 0, screen_height = 0, screen_pitch = 0;
uint32_t *lfb_address = 0;

static inline uint8_t inb(uint16_t port) {
	uint8_t val;
	__asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
	return val;
}

void vga_print(const char *str) {
	volatile uint16_t *vga = (volatile uint16_t *)0xB8000;
	for (int i = 0; str[i] != '\0'; i++) {
		vga[i] = (uint16_t)str[i] | 0x4F00;
	}
}

void put_pixel(uint32_t x, uint32_t y, uint32_t color) {
	if (x < screen_width && y < screen_height && lfb_address != 0) {
		uint32_t *row = (uint32_t *)((uint8_t *)lfb_address + y * screen_pitch);
		row[x] = color;
	}
}

void kernel_main(uint32_t magic, uint32_t addr) {
	if (magic != 0x36d76289) {
		vga_print("ERROR: Bad Multiboot2 Magic Number!");
		return;
	}

	struct multiboot_tag *tag;
	for (tag = (struct multiboot_tag *) (uintptr_t) (addr + 8);
		 tag->type != 0;
	tag = (struct multiboot_tag *) ((uint8_t *) tag + ((tag->size + 7) & ~7))) {

		if (tag->type == 8) {
			struct multiboot_tag_framebuffer *fb = (struct multiboot_tag_framebuffer *) tag;
			screen_width  = fb->framebuffer_width;
			screen_height = fb->framebuffer_height;
			screen_pitch  = fb->framebuffer_pitch;
			lfb_address   = (uint32_t *) (uintptr_t) fb->framebuffer_addr;
			break;
		}
	}

	if (lfb_address == 0) {
		vga_print("ERROR: LFB! Check grub.cfg");
		while (1) { __asm__ volatile ("hlt"); }
	}

	for (uint32_t y = 0; y < 100; y++) {
		for (uint32_t x = 0; x < 100; x++) {
			put_pixel(x, y, 0x00FF0000);
		}
	}
	uint8_t x = 0;
	while (1) {
		if (inb(0x64) & 1) {
			uint8_t scancode = inb(0x60);
			if (!(scancode & 0x80)) {
				x = !x;
				for (uint32_t dy = 0; dy < 50; dy++) {
					for (uint32_t dx = 0; dx < 50; dx++) {
						put_pixel(150 + dx, 150 + dy, x ? 0x0000FF00 : 0x00FF0000);
					}
				}
			}
		}
	}
}
