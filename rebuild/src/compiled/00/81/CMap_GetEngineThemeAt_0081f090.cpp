#include "candidates/world_map_theme.h"

namespace
{
struct ThemePaletteEntry
{
    unsigned long themeId;
    unsigned long reserved;
};

struct QuarterThemeGrid
{
    unsigned char padding_00[0x04];
    unsigned char* data;
    unsigned char padding_08[0x14];
    long width;
    long height;
};

struct GameMapThemeGrid
{
    unsigned char padding_00[0x10];
    unsigned char* data;
    unsigned char padding_14[0x1F0];
    long width;
    long maxY;
};

struct CMapThemeStorage
{
    unsigned char padding_00[0x1A6C];
    ThemePaletteEntry* paletteBegin;
    ThemePaletteEntry* paletteEnd;
    ThemePaletteEntry* paletteCapacity;
    QuarterThemeGrid* quarterGrid;
    GameMapThemeGrid* gameGrid;
};
}

FABLE_STATIC_ASSERT(offsetof(CMapThemeStorage, paletteBegin) == 0x1A6C);
FABLE_STATIC_ASSERT(offsetof(CMapThemeStorage, quarterGrid) == 0x1A78);
FABLE_STATIC_ASSERT(offsetof(CMapThemeStorage, gameGrid) == 0x1A7C);
FABLE_STATIC_ASSERT(offsetof(QuarterThemeGrid, data) == 0x04);
FABLE_STATIC_ASSERT(offsetof(QuarterThemeGrid, width) == 0x1C);
FABLE_STATIC_ASSERT(offsetof(QuarterThemeGrid, height) == 0x20);
FABLE_STATIC_ASSERT(offsetof(GameMapThemeGrid, data) == 0x10);
FABLE_STATIC_ASSERT(offsetof(GameMapThemeGrid, width) == 0x204);
FABLE_STATIC_ASSERT(offsetof(GameMapThemeGrid, maxY) == 0x208);

unsigned long CMap::GetEngineThemeAt(
    long x,
    long y,
    unsigned long themeValue,
    EThemePaintType paintType) const
{
    const CMapThemeStorage* storage =
        reinterpret_cast<const CMapThemeStorage*>(this);
    if (paintType == EThemePaintType_Default)
    {
        const GameMapThemeGrid* grid = storage->gameGrid;
        const unsigned char paletteIndex = grid->data[
            ((grid->maxY - y) * (grid->width + 1) + x) * 0x0C + 4 + themeValue];
        return storage->paletteBegin[paletteIndex].themeId;
    }
    if (paintType == EThemePaintType_Replace)
    {
        const QuarterThemeGrid* grid = storage->quarterGrid;
        return grid->data[
            ((grid->height - y / 4 - 1) * grid->width + x / 4) * 7 + themeValue];
    }
    return 0;
}
