#include "candidates/world_map_theme.h"

namespace
{
struct QuarterBlendWriteGrid
{
    unsigned char padding_00[0x04]; unsigned char* data;
    unsigned char padding_08[0x14]; long width; long height;
};
struct GameBlendWriteGrid
{
    unsigned char padding_00[0x10]; unsigned char* data;
    unsigned char padding_14[0x1F0]; long width; long maxY;
};
struct CMapBlendWriteStorage
{
    unsigned char padding_00[0x1A78];
    QuarterBlendWriteGrid* quarterGrid;
    GameBlendWriteGrid* gameGrid;
};
}

FABLE_STATIC_ASSERT(offsetof(CMapBlendWriteStorage, quarterGrid) == 0x1A78);
FABLE_STATIC_ASSERT(offsetof(CMapBlendWriteStorage, gameGrid) == 0x1A7C);

void CMap::SetEngineBlendAt(
    long x, long y, unsigned char blendValue, unsigned long blendIndex,
    EThemePaintType paintType)
{
    CMapBlendWriteStorage* storage = reinterpret_cast<CMapBlendWriteStorage*>(this);
    if (paintType == EThemePaintType_Default)
    {
        GameBlendWriteGrid* grid = storage->gameGrid;
        grid->data[
            ((grid->maxY - y) * (grid->width + 1) + x) * 0x0C + 7 + blendIndex] =
            blendValue;
        return;
    }
    if (paintType == EThemePaintType_Replace)
    {
        QuarterBlendWriteGrid* grid = storage->quarterGrid;
        grid->data[
            (((grid->height - ((y + ((y >> 31) & 3)) >> 2)) - 1) * grid->width +
             ((x + ((x >> 31) & 3)) >> 2)) * 7 + 3 + blendIndex] = blendValue;
    }
}
