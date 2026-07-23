#include <stdio.h>
#include <string.h>
#include "candidates/world_map_theme.h"

static char g_calls[16];
static int g_callCount;
static CMap g_map;
static long g_mapNumber;
static int g_canPaint;
static int g_getMapSawNumber;
static long g_setX;
static long g_setY;
static unsigned char g_setBlendValue;
static unsigned long g_setThemeId;
static EThemePaintType g_setPaintType;

static void Record(char call)
{
    g_calls[g_callCount++] = call;
    g_calls[g_callCount] = 0;
}

static long FABLE_FASTCALL GetMapNumberAtShim(
    CWorldMap*,
    void*,
    const C2DCoordI&)
{
    Record('R');
    return g_mapNumber;
}

static CMap* FABLE_FASTCALL GetMapShim(CWorldMap*, void*, long mapNumber)
{
    Record('G');
    g_getMapSawNumber = mapNumber == g_mapNumber;
    return &g_map;
}

static bool FABLE_FASTCALL IsPosChangeableShim(
    CWorldMap*,
    void*,
    const C2DCoordI&)
{
    Record('P');
    return g_canPaint != 0;
}

C2DCoordI CWorldMap::ConvertWorldMapCoordsToLocalCoords(
    long mapNumber,
    const C2DCoordI& worldMapCoord) const
{
    Record('C');
    C2DCoordI localCoord;
    localCoord.x = worldMapCoord.x - mapNumber;
    localCoord.y = worldMapCoord.y - mapNumber * 2;
    return localCoord;
}

void CMap::SetEngineBlendAt(
    long x,
    long y,
    unsigned char blendValue,
    unsigned long themeId,
    EThemePaintType paintType)
{
    Record('S');
    g_setX = x;
    g_setY = y;
    g_setBlendValue = blendValue;
    g_setThemeId = themeId;
    g_setPaintType = paintType;
}

static void Reset()
{
    memset(g_calls, 0, sizeof(g_calls));
    g_callCount = 0;
    g_mapNumber = 5;
    g_canPaint = 1;
    g_getMapSawNumber = 0;
    g_setX = 0;
    g_setY = 0;
    g_setBlendValue = 0;
    g_setThemeId = 0;
    g_setPaintType = EThemePaintType_Default;
}

int main()
{
    void* vtable[26];
    memset(vtable, 0, sizeof(vtable));
    vtable[0x44 / 4] = (void*)&GetMapNumberAtShim;
    vtable[0x48 / 4] = (void*)&GetMapShim;
    vtable[0x64 / 4] = (void*)&IsPosChangeableShim;

    struct FakeWorldMap
    {
        void** vtable;
    } fake;
    fake.vtable = vtable;
    CWorldMap* const worldMap = reinterpret_cast<CWorldMap*>(&fake);
    C2DCoordI worldCoord;
    worldCoord.x = 37;
    worldCoord.y = 81;

    Reset();
    g_mapNumber = 0;
    worldMap->SetThemeBlendAtBlock(worldCoord, 91, 12, EThemePaintType_Replace);
    if (strcmp(g_calls, "R") != 0) return 1;

    Reset();
    g_canPaint = 0;
    worldMap->SetThemeBlendAtBlock(worldCoord, 91, 12, EThemePaintType_Replace);
    if (strcmp(g_calls, "RP") != 0) return 2;

    Reset();
    worldMap->SetThemeBlendAtBlock(worldCoord, 91, 12, EThemePaintType_Replace);
    if (strcmp(g_calls, "RPCGS") != 0) return 3;
    if (!g_getMapSawNumber) return 4;
    if (g_setX != 32 || g_setY != 71) return 5;
    if (g_setBlendValue != 91 || g_setThemeId != 12) return 6;
    if (g_setPaintType != EThemePaintType_Replace) return 7;

    printf("WORLD_MAP_SET_THEME_BLEND_TEST PASS\n");
    return 0;
}
