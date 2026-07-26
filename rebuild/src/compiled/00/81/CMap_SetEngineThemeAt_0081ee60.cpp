#include "candidates/world_map_theme.h"

namespace
{
struct QuarterThemeWriteGrid
{
    unsigned char padding_00[0x04]; unsigned char* data;
    unsigned char padding_08[0x14]; long width; long height;
};
struct GameThemeWriteGrid
{
    unsigned char padding_00[0x10]; unsigned char* data;
    unsigned char padding_14[0x1F0]; long width; long maxY;
};
struct CMapThemeWriteStorage
{
    unsigned char padding_00[0x1A78];
    QuarterThemeWriteGrid* quarterGrid;
    GameThemeWriteGrid* gameGrid;
};
}

FABLE_STATIC_ASSERT(offsetof(CMapThemeWriteStorage, quarterGrid) == 0x1A78);
FABLE_STATIC_ASSERT(offsetof(CMapThemeWriteStorage, gameGrid) == 0x1A7C);

void CMap::SetEngineThemeAt(
    long x, long y, unsigned long themeId, unsigned long themeValue,
    EThemePaintType paintType)
{
    CMapThemeWriteStorage* storage = reinterpret_cast<CMapThemeWriteStorage*>(this);
    if (paintType != EThemePaintType_Default)
    {
        if (paintType == EThemePaintType_Replace)
        {
            QuarterThemeWriteGrid* grid = storage->quarterGrid;
            grid->data[
                (((grid->height - ((y + ((y >> 31) & 3)) >> 2)) - 1) * grid->width +
                 ((x + ((x >> 31) & 3)) >> 2)) * 7 + themeValue] =
                static_cast<unsigned char>(themeId);
        }
        return;
    }

    GameThemeWriteGrid* grid = storage->gameGrid;
    const unsigned long offset =
        ((grid->maxY - y) * (grid->width + 1) + x) * 0x0C + 4 + themeValue;
    if (themeId != 0)
        grid->data[offset] = AddThemeDefIndexToPalette(themeId);
    else
        grid->data[offset] = 0;
}
