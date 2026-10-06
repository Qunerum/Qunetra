#ifndef MATH_H
#define MATH_H

#include "types.h"

static inline state getBit(const uint8 val, const uint8 bit_index) {
	if (bit_index > 7) return false;
	return (val >> bit_index) & 1;
}
static inline uint8 setBit(const uint8 val, const uint8 bit_index) {
	if (bit_index > 7) return 0;
	return val | (1 << bit_index);
}
static inline uint8 clearBit(const uint8 val, uint8 bit_index) {
	if (bit_index > 7) return 0;
	return val & ~(1 << bit_index);
}

#endif
