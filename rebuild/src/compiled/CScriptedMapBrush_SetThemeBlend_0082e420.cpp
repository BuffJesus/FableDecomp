#include "candidates/scripted_map_brush_theme.h"

void CScriptedMapBrush::SetThemeBlend(
    long x, long y, unsigned char blend, unsigned long layer)
{
    if (x >= minX)
        if (x < maxX)
            if (y >= minY)
                if (y < maxY)
                    reinterpret_cast<unsigned char*>(cellsData)[
                        (((((y - minY) * (maxX - minX)) - minX) + x) * 0x14) +
                        0x10 + layer] = blend;
}
