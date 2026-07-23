#include "candidates/scripted_map_brush_theme.h"

unsigned long CScriptedMapBrush::GetTheme(long x, long y, unsigned long layer) const
{
    if (x >= minX)
        if (x < maxX)
            if (y >= minY)
                if (y < maxY)
                    return cellsData[
                        1 + (layer +
                             ((((y - minY) * (maxX - minX)) - minX) + x) * 5)];
    return 0;
}
