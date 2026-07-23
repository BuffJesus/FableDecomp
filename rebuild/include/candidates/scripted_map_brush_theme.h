#ifndef FABLETLC_SCRIPTED_MAP_BRUSH_THEME_H
#define FABLETLC_SCRIPTED_MAP_BRUSH_THEME_H

#include <stddef.h>
#include "rebuild_abi.h"

class CScriptedMapBrush
{
public:
    unsigned char padding_00[0x08];
    unsigned long* cellsData;
    unsigned char padding_0C[0x2C];
    long minX;
    long minY;
    long maxX;
    long maxY;

    unsigned long GetTheme(long x, long y, unsigned long layer) const;
    unsigned char GetThemeBlend(long x, long y, unsigned long layer) const;
    void SetThemeBlend(long x, long y, unsigned char blend, unsigned long layer);
};

FABLE_STATIC_ASSERT(offsetof(CScriptedMapBrush, cellsData) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CScriptedMapBrush, minX) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CScriptedMapBrush, minY) == 0x3C);
FABLE_STATIC_ASSERT(offsetof(CScriptedMapBrush, maxX) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CScriptedMapBrush, maxY) == 0x44);

#endif
