#include <stdio.h>
#include <string.h>
#include "candidates/landscape_patch_update_foreground_compiled.h"

fable_u8* DAT_01436ea0;
fable_u8* DAT_01436ea8;
fable_u8 DAT_013d2880;

static int g_deleteCalls, g_clearCalls, g_discardCalls, g_loadTextureCalls;
static int g_loadForegroundCalls, g_backgroundCalls;
static LS_LOADING_STATUS g_loadStatus;
static bool g_texturesReady, g_backgroundReady;
static float g_lastDistance;

bool CEngineLandscapeMap::IsBackgroundAtFullDetailInArea(
    unsigned long x, unsigned long y, unsigned long width, unsigned long height)
{
    ++g_backgroundCalls;
    return g_backgroundReady && x == 7 && y == 9 && width == 0x10 && height == 0x10;
}

void CWaterPatchMesh::ClearTextures() { ++g_clearCalls; }
void CBankFileAsyncData::DiscardAsyncData() { ++g_discardCalls; }
void CEngineLandscapePatch::DeleteLayerMeshes() { ++g_deleteCalls; }

LS_LOADING_STATUS CEngineLandscapePatch::LoadForegroundPatch(
    CCountedPointer<CBankFileAsyncEntry>)
{
    ++g_loadForegroundCalls;
    return g_loadStatus;
}

bool CEngineLandscapePatch::LoadTextures(float distanceSquared)
{
    ++g_loadTextureCalls;
    g_lastDistance = distanceSquared;
    if (g_texturesReady)
        Flags = static_cast<fable_u8>(Flags | 1);
    return g_texturesReady;
}

class TestReleaseTarget : public CCountedReleaseTarget
{
public:
    void Release() {}
};

static void Reset(CEngineLandscapePatch& patch, CEngineLandscapeMap& map, CWaterPatchMesh& water)
{
    memset(&patch, 0, sizeof(patch));
    patch.LandscapeMap = &map;
    patch.WaterPatch = &water;
    patch.BoundsMinX = patch.BoundsMinY = patch.BoundsMinZ = -1.0f;
    patch.BoundsMaxX = patch.BoundsMaxY = patch.BoundsMaxZ = 1.0f;
    patch.PatchX = 7;
    patch.PatchY = 9;
    g_deleteCalls = g_clearCalls = g_discardCalls = g_loadTextureCalls = 0;
    g_loadForegroundCalls = g_backgroundCalls = 0;
    g_loadStatus = LS_LOADING_READY;
    g_texturesReady = g_backgroundReady = true;
    g_lastDistance = 0.0f;
}

int main()
{
    fable_u8 camera[0x60];
    fable_u8 globals[0x620];
    memset(camera, 0, sizeof(camera));
    memset(globals, 0, sizeof(globals));
    DAT_01436ea0 = camera;
    DAT_01436ea8 = globals;
    *reinterpret_cast<float*>(globals + 0x618) = 10.0f;

    CEngineLandscapePatch patch;
    CEngineLandscapeMap map;
    CWaterPatchMesh water;
    CBankFileAsyncEntry entry;
    TestReleaseTarget releaseTarget;
    CCountedReference* reference = static_cast<CCountedReference*>(operator new(sizeof(CCountedReference)));
    reference->Count = 0;
    reference->Release = static_cast<CCountedReleaseFunction>(&TestReleaseTarget::Release);
    reference->Owner = &releaseTarget;
    CCountedPointer<CBankFileAsyncEntry> counted(&entry, reference);

    Reset(patch, map, water);
    patch.Flags = 0;
    if (patch.UpdateForeground(counted) != LS_LOADING_READY) return 1;
    if (g_loadTextureCalls || reference->Count != 1) return 2;

    Reset(patch, map, water);
    patch.Flags = 0x18;
    patch.ForegroundDataSize = 4;
    *reinterpret_cast<float*>(camera + 0x54) = 20.0f;
    if (patch.UpdateForeground(counted) != LS_LOADING_READY) return 3;
    if (g_deleteCalls != 1 || g_clearCalls != 1 || g_discardCalls != 1) return 4;
    if (patch.Flags != 0x10 || reference->Count != 1) return 5;

    Reset(patch, map, water);
    patch.Flags = 0x10;
    patch.ForegroundDataSize = 4;
    *reinterpret_cast<float*>(camera + 0x54) = 0.0f;
    g_loadStatus = LS_LOADING_PENDING;
    if (patch.UpdateForeground(counted) != LS_LOADING_PENDING) return 6;
    if (g_loadForegroundCalls != 1 || g_loadTextureCalls != 0 || reference->Count != 1) return 7;

    Reset(patch, map, water);
    patch.Flags = 0x10;
    patch.ForegroundDataSize = 4;
    g_loadStatus = LS_LOADING_FAILED;
    if (patch.UpdateForeground(counted) != LS_LOADING_FAILED) return 8;
    if (g_deleteCalls != 1 || g_clearCalls != 1 || reference->Count != 1) return 9;

    Reset(patch, map, water);
    patch.Flags = 0x10;
    g_texturesReady = true;
    g_backgroundReady = true;
    if (patch.UpdateForeground(counted) != LS_LOADING_READY) return 10;
    if (g_loadTextureCalls != 1 || g_backgroundCalls != 1) return 11;
    if ((patch.Flags & 0x0D) != 0x0D || reference->Count != 1) return 12;

    printf("LANDSCAPE_PATCH_UPDATE_FOREGROUND_TEST PASS\n");
    return 0;
}
