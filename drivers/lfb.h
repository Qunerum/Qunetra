#ifndef LFB_H
#define LFB_H

#include "../utility/types.h"

void initLFB(const uint32 addr);
void putPx(const uint32_t x, const uint32_t y, const uint32_t color);
void drawRect(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height, const uint32_t color);
void lfbClear(const uint32_t color);

#endif
