#include <stdarg.h>
#include "../utility/types.h"
#include "../drivers/lfb.h"
#include "font.h"

#define PALETTE_MAX 256
static const uint32 palette[PALETTE_MAX] = {
	// --- 00 - 15 | Grayscale ---
	0x000000, 0x111111, 0x222222, 0x333333, 0x444444, 0x555555, 0x666666, 0x777777,
	0x888888, 0x999999, 0xAAAAAA, 0xBBBBBB, 0xCCCCCC, 0xDDDDDD, 0xEEEEEE, 0xFFFFFF,
	// --- 16 - 23 | Deep Crimson ---
	0x400000, 0x5B0000, 0x760000, 0x910000, 0xAC0000, 0xC70000, 0xE20000, 0xFF1A1A,
	// --- 24 - 31 | True Red ---
	0x500A0A, 0x6B1212, 0x861B1B, 0xA12323, 0xBC2B2B, 0xD73333, 0xF23B3B, 0xFF4D4D,
	// --- 32 - 39 | Scarlet / Tomato ---
	0x601500, 0x7B2100, 0x962E00, 0xB13A00, 0xCC4700, 0xE75300, 0xFF6000, 0xFF7F33,
	// --- 40 - 47 | Rich Orange ---
	0x652200, 0x832C00, 0xA13700, 0xBF4100, 0xDD4C00, 0xFF5800, 0xFF751A, 0xFF9233,
	// --- 48 - 55 | Amber / Gold-Orange ---
	0x6E3500, 0x8C4400, 0xAA5300, 0xC86200, 0xE67100, 0xFF8000, 0xFF991A, 0xFFB333,
	// --- 56 - 63 | Dark Chocolate Brown ---
	0x331A0D, 0x472412, 0x5C2E16, 0x70381B, 0x85421F, 0x994C24, 0xAD5728, 0xC2612D,
	// --- 64 - 71 | Warm Earth Brown ---
	0x402010, 0x592D17, 0x723A1E, 0x8C4725, 0xA5552C, 0xBE6233, 0xD76F3A, 0xF07C41,
	// --- 72 - 79 | Light Tan / Caramel ---
	0x53391A, 0x6B4A22, 0x845B2B, 0x9C6C33, 0xB57D3C, 0xCD8E44, 0xE69F4D, 0xFFB055,
	// --- 80 - 87 | Dark Mustard / Ochre ---
	0x4D3D00, 0x695300, 0x856A00, 0xA18000, 0xBD9700, 0xD9AE00, 0xF5C500, 0xFFD71A,
	// --- 88 - 95 | Golden Yellow ---
	0x554800, 0x736100, 0x917A00, 0xAF9200, 0xCDAB00, 0xEBC400, 0xFFDD00, 0xFFE633,
	// --- 96 - 103 | Lemon Yellow ---
	0x555500, 0x777700, 0x999900, 0xBBBB00, 0xDDDD00, 0xFFFF00, 0xFFFF33, 0xFFFF66,
	// --- 104 - 111 | Chartreuse / Yellow-Green ---
	0x3A5500, 0x4E7300, 0x639100, 0x77AF00, 0x8BCD00, 0xA0EB00, 0xB5FF00, 0xC8FF33,
	// --- 112 - 119 | Olive Green ---
	0x263300, 0x384D00, 0x4B6600, 0x5D8000, 0x709900, 0x82B300, 0x95CC00, 0xA7E600,
	// --- 120 - 127 | Deep Forest Green ---
	0x002600, 0x003D00, 0x005500, 0x006C00, 0x008400, 0x009B00, 0x00B300, 0x00CC1A,
	// --- 128 - 135 | Classic Green ---
	0x003311, 0x004D1A, 0x006622, 0x00802B, 0x009933, 0x00B33B, 0x00CC44, 0x00E64D,
	// --- 136 - 143 | Vibrant Lime Green ---
	0x004010, 0x006018, 0x008020, 0x00A028, 0x00C030, 0x00E038, 0x00FF40, 0x33FF66,
	// --- 144 - 151 | Mint / Spring Green ---
	0x004D26, 0x007338, 0x00994C, 0x00BF5F, 0x00E673, 0x1AFF88, 0x4DFF99, 0x80FFB3,
	// --- 152 - 159 | Emerald / Teal-Green ---
	0x004033, 0x00604D, 0x008066, 0x00A080, 0x00C099, 0x00E0B3, 0x00FFCC, 0x4DFFE6,
	// --- 160 - 167 | Deep Cyan / Dark Turquoise ---
	0x003340, 0x004D60, 0x006680, 0x008099, 0x0099B3, 0x00B3CC, 0x00CCE6, 0x00FFFF,
	// --- 168 - 175 | Bright Cyan / Aqua ---
	0x003A4D, 0x005973, 0x007999, 0x0098BF, 0x00B8E6, 0x00D7FF, 0x33E2FF, 0x66ECFF,
	// --- 176 - 183 | Ice Blue / Soft Cyan ---
	0x1A3A4D, 0x265573, 0x337199, 0x3F8CBF, 0x4CA8E5, 0x58C4FF, 0x7AD1FF, 0x9DDDFF,
	// --- 184 - 191 | Deep Navy Blue ---
	0x000033, 0x00004D, 0x000066, 0x000080, 0x000099, 0x0000B3, 0x0000CC, 0x001AE6,
	// --- 192 - 199 | Classic Royal Blue ---
	0x001A40, 0x002B66, 0x003D8C, 0x004EB3, 0x005FD9, 0x0070FF, 0x338DFF, 0x66ABFF,
	// --- 200 - 207 | Electric Sky Blue ---
	0x002B55, 0x004280, 0x0059AB, 0x0070D6, 0x0088FF, 0x2EA2FF, 0x5CB8FF, 0x8CCFFF,
	// --- 208 - 215 | Cobalt / Indigo Blue ---
	0x140040, 0x1F0060, 0x2A0080, 0x35009F, 0x4000BF, 0x4B00DF, 0x5700FF, 0x7A33FF,
	// --- 216 - 223 | Deep Purple / Plum ---
	0x260033, 0x3B004D, 0x4F0066, 0x640080, 0x780099, 0x8D00B3, 0xA100CC, 0xB61AE6,
	// --- 224 - 231 | Classic Violet ---
	0x330033, 0x4D004D, 0x660066, 0x800080, 0x990099, 0xB300B3, 0xCC00CC, 0xE600E6,
	// --- 232 - 239 | Orchid / Bright Purple ---
	0x400040, 0x600060, 0x800080, 0xA000A0, 0xC000C0, 0xE000E0, 0xFF00FF, 0xFF33FF,
	// --- 240 - 247 | Deep Magenta / Wine-Pink ---
	0x4D0026, 0x730038, 0x99004D, 0xBF0060, 0xE60073, 0xFF0080, 0xFF3399, 0xFF66B3,
	// --- 248 - 255 | Hot Pink / Neon Rose ---
	0x4D0033, 0x73004D, 0x990066, 0xBF0080, 0xE60099, 0xFF00B3, 0xFF33CC, 0xFF66E6
};

static uint32 mulPx = 1, charColor = 0xDDDDDD, cursorX = 0, cursorY = 0;
void setCharSize(const uint32 newSize) { mulPx = newSize; }
void setCharColor(const uint8 newColor) { charColor = palette[newColor]; }
void putChar(const char c) {
	if (c == '\0') return;
	if (c == ' ') { cursorX++; return; }
	if (c == '\n') {
		cursorX = 0;
		cursorY++;
		return;
	}
	if (c == '\b') {
		if (cursorX == 0) {
			if (cursorY == 0) return;
			cursorY--;
		} else cursorX--;
		drawRect(cursorX * qFontX * mulPx, cursorY * qFontY * mulPx, qFontX * mulPx, qFontY * mulPx, 0);
		return;
	}
	uint32 baseX = cursorX * qFontX * mulPx, baseY = cursorY * qFontY * mulPx;
	for (uint8 ly = 0; ly < qFontY; ly++) {
		int8 x = 0;
		for (uint8 lx = 0; lx < qFontMax; lx++) {
			int8 v = defaultFont[(uint8)c][ly][lx];
			if (v == 0) break;
			state isEnd = (v < 0);
			if (isEnd) v = -v;
			if (v > 10) {
				uint8 w = v - 10;
				uint32 px = baseX + (x * mulPx), py = baseY + (ly * mulPx);
				drawRect(px, py, w * mulPx, mulPx, charColor);
				x += w;
			} else x += v;
			if (isEnd) break;
		}
	}
	cursorX++;
}

static void prtDec(int v) {
	if (v == 0) {
		putChar('0');
		return;
	}
	if (v < 0) {
		putChar('-');
		v = -v;
	}
	char buf[32];
	int i = 0;
	while (v > 0) {
		buf[i++] = '0' + (v % 10);
		v /= 10;
	}
	while (i > 0) putChar(buf[--i]);
}
static void prtFlt(double v, const uint precision) {
	if (v < 0) {
		putChar('-');
		v = -v;
	}
	long long int_part = (long long)v;
	prtDec((int)int_part);
	if (precision > 0) {
		putChar('.');
		double frac = v - (double)int_part;
		long long mult = 1;
		for (uint i = 0; i < precision; i++) mult *= 10;
		long long frac_part = (long long)((frac * mult) + 0.5);
		prtDec((int)frac_part);
	}
}
static const char hex_chars[] = "0123456789ABCDEF";
void kprintf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	charColor = palette[13];
	for (const char *p = format; *p != '\0'; p++) {
		if (*p != '%') {
			putChar(*p);
			continue;
		}
		p++;
		uint precision = 6;
		if (*p == '.') {
			p++;
			if (*p >= '0' && *p <= '9') {
				precision = *p - '0';
				p++;
			}
		}
		switch (*p) {
			case 'q': {
				int v = va_arg(args, int);
				if (0 <= v && v < PALETTE_MAX) charColor = palette[v];
				break;
			}
			case 'm': {
				int v = va_arg(args, int);
				if (v) {
					putChar('t');
					putChar('r');
					putChar('u');
					putChar('e');
				} else {
					putChar('f');
					putChar('a');
					putChar('l');
					putChar('s');
					putChar('e');
				}
				break;
			}

			case 'd': {
				int v = va_arg(args, int);
				prtDec(v);
				break;
			}
			case 's': {
				const char *s = va_arg(args, const char *);
				if (!s) s = "(null)";
				while (*s) putChar(*s++);
				break;
			}
			case 'c': {
				char c = (char)va_arg(args, int);
				putChar(c);
				break;
			}
			case 'f': {
				double v = va_arg(args, double);
				prtFlt(v, precision);
				break;
			}
			case 'b': {
				uint v = va_arg(args, uint);
				if (v == 0) {
					putChar('0');
					break;
				}
				state started = false;
				for (int8 i = 31; i >= 0; i--) {
					state bit = (v >> i) & 1;
					if (bit) started = true;
					if (started || i == 0) putChar(bit ? '1' : '0');
				}
				break;
			}
			case 'h': {
				uint v = va_arg(args, uint);
				if (v == 0) {
					putChar('0');
					break;
				}
				char buffer[8];
				uint idx = 0, temp = v;
				while (temp > 0) {
					buffer[idx++] = hex_chars[temp & 0xF];
					temp >>= 4;
				}
				for (int i = idx - 1; i >= 0; i--) putChar(buffer[i]);
				break;
			}
			case '%': {
				putChar('%');
				break;
			}
			default:
				putChar('%');
				putChar(*p);
				break;
		}
	}
	va_end(args);
}
