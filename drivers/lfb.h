#ifndef LFB_H
#define LFB_H

#include "../utility/types.h"

void initLFB(const uint32 addr);
void scrollUp();
void scrollUpN(uint32 n);
void putPx(const uint32 x, const uint32 y, const uint32 color);
void drawRect(const uint32 x, const uint32 y, const uint32 width, const uint32 height, const uint32 color);
void lfbClear(const uint32 color);

#endif
