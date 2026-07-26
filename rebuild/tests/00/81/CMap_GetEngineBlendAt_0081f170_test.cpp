#include <stdio.h>
#include <string.h>
#include "candidates/world_map_theme.h"

class CGameMapCell
{
public:
    unsigned char GetAtmosBlend(unsigned long blendIndex) const;
};

unsigned char CGameMapCell::GetAtmosBlend(unsigned long blendIndex) const
{
    return reinterpret_cast<const unsigned char*>(this)[3 + blendIndex];
}

struct QuarterGrid {
    unsigned char padding_00[4]; unsigned char* data; unsigned char padding_08[0x14];
    long width; long height;
};
struct GameGrid {
    unsigned char padding_00[0x10]; unsigned char* data; unsigned char padding_14[0x1F0];
    long width; long maxY;
};

int main()
{
    unsigned char object[0x1A80];
    QuarterGrid quarter; GameGrid game;
    unsigned char quarterData[7 * 16]; unsigned char gameData[12 * 25];
    memset(object, 0, sizeof(object)); memset(&quarter, 0, sizeof(quarter));
    memset(&game, 0, sizeof(game)); memset(quarterData, 0, sizeof(quarterData));
    memset(gameData, 0, sizeof(gameData));
    *reinterpret_cast<QuarterGrid**>(object + 0x1A78) = &quarter;
    *reinterpret_cast<GameGrid**>(object + 0x1A7C) = &game;
    quarter.data = quarterData; quarter.width = 4; quarter.height = 4;
    game.data = gameData; game.width = 4; game.maxY = 4;
    gameData[((4 - 2) * 5 + 1) * 12 + 7 + 2] = 0x72;
    quarterData[(((4 - 1) - 1) * 4 + 2) * 7 + 3 + 2] = 0x94;
    const CMap* map = reinterpret_cast<const CMap*>(object);
    if (map->GetEngineBlendAt(1, 2, 2, EThemePaintType_Default) != 0x72) return 1;
    if (map->GetEngineBlendAt(8, 4, 2, EThemePaintType_Replace) != 0x94) return 2;
    if (map->GetEngineBlendAt(1, 2, 2, static_cast<EThemePaintType>(2)) != 0) return 3;
    printf("MAP_GET_ENGINE_BLEND_TEST PASS\n");
    return 0;
}
