#include "candidates/scripted_map_brush_theme.h"

unsigned char CScriptedMapBrush::GetThemeBlend(
    long x, long y, unsigned long layer) const
{
    if (x >= minX)
        if (x < maxX)
            if (y >= minY)
                if (y < maxY)
                    return reinterpret_cast<const unsigned char*>(cellsData)[
                        (((((y - minY) * (maxX - minX)) - minX) + x) * 0x14) +
                        0x10 + layer];
    return 0;
}
