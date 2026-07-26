#include <stdio.h>
#include <string.h>
#include "candidates/landscape_patch_compiled.h"

fable_u8* DAT_01436e98;
fable_u8* DAT_01436ea8;

static int g_addCalls;
static fable_u16* g_indices;
static fable_u32 g_count;
static float g_distance;

void CEngineLandscapeRenderer::AddFrameForegroundTexturesUsed(
    fable_u8*,
    fable_u16* textureIndices,
    fable_u32 textureCount,
    float distanceSquared)
{
    ++g_addCalls;
    g_indices = textureIndices;
    g_count = textureCount;
    g_distance = distanceSquared;
}
static void StorePointer(fable_u8* destination, void* value)
{
    *reinterpret_cast<void**>(destination) = value;
}

static int RunCase(bool ready, int expectedResult)
{
    fable_u8 globals[0x958];
    fable_u8 manager[0x1E8];
    fable_u8 entries[0x58];
    fable_u8 handles[0x20];
    fable_u8 resources[0x40];
    fable_u16 indices[2] = { 0, 1 };
    CEngineLandscapePatch patch;
    memset(globals, 0, sizeof(globals));
    memset(manager, 0, sizeof(manager));
    memset(entries, 0, sizeof(entries));
    memset(handles, 0, sizeof(handles));
    memset(resources, 0, sizeof(resources));
    memset(&patch, 0, sizeof(patch));

    StorePointer(globals + 0x954, manager);
    *reinterpret_cast<fable_u32*>(manager + 0x1E0) = reinterpret_cast<fable_u32>(entries);
    *reinterpret_cast<fable_u32*>(manager + 0x1E4) = reinterpret_cast<fable_u32>(entries + sizeof(entries));
    StorePointer(entries + 0x28, handles + 0x00);
    StorePointer(entries + 0x2C + 0x28, handles + 0x10);
    StorePointer(handles + 0x08, resources + 0x00);
    StorePointer(handles + 0x10 + 0x08, resources + 0x20);
    *reinterpret_cast<fable_i32*>(resources + 0x1C) = 1;
    *reinterpret_cast<fable_i32*>(resources + 0x20 + 0x1C) = ready ? 1 : 0;

    DAT_01436e98 = globals;
    DAT_01436ea8 = globals + 0x100;
    patch.m_pForegroundTextureIndices = indices;
    patch.m_nForegroundTextureCount = 2;
    patch.m_Flags = 0xA0;
    g_addCalls = 0;
    g_indices = 0;
    g_count = 0;
    g_distance = 0.0f;

    const bool result = patch.LoadTextures(42.5f);
    if (result != (expectedResult != 0)) return 1;
    if (g_addCalls != 1) return 2;
    if (g_indices != indices || g_count != 2 || g_distance != 42.5f) return 3;
    if (((patch.m_Flags & 1) != 0) != (expectedResult != 0)) return 4;
    return 0;
}

int main()
{
    const int ready = RunCase(true, 1);
    if (ready != 0)
    {
        printf("LOADTEXTURES_TEST FAIL ready=%d\n", ready);
        return 100 + ready;
    }
    const int unavailable = RunCase(false, 0);
    if (unavailable != 0)
    {
        printf("LOADTEXTURES_TEST FAIL unavailable=%d\n", unavailable);
        return 200 + unavailable;
    }
    printf("LOADTEXTURES_TEST PASS\n");
    return 0;
}
