#include "candidates/world_map_theme.h"

class CGameMapCell
{
public:
    unsigned char GetAtmosBlend(unsigned long blendIndex) const;
};

namespace
{
struct QuarterBlendGrid
{
    unsigned char padding_00[0x04];
    unsigned char* data;
    unsigned char padding_08[0x14];
    long width;
    long height;
};

struct GameMapBlendGrid
{
    unsigned char padding_00[0x10];
    unsigned char* data;
    unsigned char padding_14[0x1F0];
    long width;
    long maxY;
};

struct CMapBlendStorage
{
    unsigned char padding_00[0x1A78];
    QuarterBlendGrid* quarterGrid;
    GameMapBlendGrid* gameGrid;
};
}

FABLE_STATIC_ASSERT(offsetof(CMapBlendStorage, quarterGrid) == 0x1A78);
FABLE_STATIC_ASSERT(offsetof(CMapBlendStorage, gameGrid) == 0x1A7C);

unsigned char CMap::GetEngineBlendAt(
    long x,
    long y,
    unsigned long blendIndex,
    EThemePaintType paintType) const
{
    const CMapBlendStorage* storage =
        reinterpret_cast<const CMapBlendStorage*>(this);
    if (paintType == EThemePaintType_Default)
    {
        const GameMapBlendGrid* grid = storage->gameGrid;
        return grid->data[
            ((grid->maxY - y) * (grid->width + 1) + x) * 0x0C + 7 + blendIndex];
    }
    if (paintType == EThemePaintType_Replace)
    {
        const QuarterBlendGrid* grid = storage->quarterGrid;
        return reinterpret_cast<const CGameMapCell*>(
            grid->data +
            (((grid->height - ((y + ((y >> 31) & 3)) >> 2)) - 1) * grid->width +
             ((x + ((x >> 31) & 3)) >> 2)) * 7)->GetAtmosBlend(blendIndex);
    }
    return 0;
}
