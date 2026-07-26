#include <new>
#include <stdio.h>
#include <string.h>
#include "candidates/landscape_map_update_foreground_compiled.h"

fable_u8* DAT_01436ea0;
fable_u8* DAT_01436ea8;
fable_u8 DAT_013d2880;

static CBankFileAsyncData g_asyncData;
static CMapCountedReference* g_asyncReference;
static bool g_asyncFinished;
static fable_u8 g_asyncBytes[0x100];
static unsigned long g_asyncSize;
static int g_beginCalls, g_discardCalls, g_asyncReleases;
static int g_ctorCalls, g_dtorCalls, g_initialiseCalls, g_loadHeaderCalls;
static int g_patchUpdateCalls, g_streamCtorCalls, g_streamDtorCalls;
static unsigned long g_lastOffset, g_lastReadSize;
static void* g_lastStreamData;
static unsigned long g_lastStreamSize;
static int g_initialiseX[8], g_initialiseY[8];
static LS_LOADING_STATUS g_patchStatuses[8];

class TestReleaseTarget : public CMapCountedReleaseTarget
{
public:
    void Release() { ++g_asyncReleases; }
};

static TestReleaseTarget g_releaseTarget;

bool CBankFileAsyncData::IsFinished() { return g_asyncFinished; }
void* CBankFileAsyncData::GetData() { return g_asyncBytes; }
unsigned long CBankFileAsyncData::GetDataSize() { return g_asyncSize; }
void CBankFileAsyncData::DiscardAsyncData() { ++g_discardCalls; }

CMapCountedPointer<CBankFileAsyncData> CBankFileAsyncEntry::BeginReadDataAutoAlloc(
    unsigned long offset,
    unsigned long size)
{
    ++g_beginCalls;
    g_lastOffset = offset;
    g_lastReadSize = size;
    g_asyncReference = static_cast<CMapCountedReference*>(operator new(sizeof(CMapCountedReference)));
    g_asyncReference->Count = 0;
    g_asyncReference->Release = static_cast<CMapCountedReleaseFunction>(&TestReleaseTarget::Release);
    g_asyncReference->Owner = &g_releaseTarget;
    return CMapCountedPointer<CBankFileAsyncData>(&g_asyncData, g_asyncReference);
}

CMemoryDataInputStream::CMemoryDataInputStream(void* data, unsigned long size)
{
    ++g_streamCtorCalls;
    g_lastStreamData = data;
    g_lastStreamSize = size;
}

CMemoryDataInputStream::~CMemoryDataInputStream() { ++g_streamDtorCalls; }

CEngineLandscapePatch::CEngineLandscapePatch() { ++g_ctorCalls; }
CEngineLandscapePatch::~CEngineLandscapePatch() { ++g_dtorCalls; }

void CEngineLandscapePatch::Initialise(CEngineLandscapeMap*, int x, int y)
{
    g_initialiseX[g_initialiseCalls] = x;
    g_initialiseY[g_initialiseCalls] = y;
    ++g_initialiseCalls;
}

void CEngineLandscapePatch::LoadHeader(CMemoryDataInputStream&) { ++g_loadHeaderCalls; }

LS_LOADING_STATUS CEngineLandscapePatch::UpdateForeground(
    CMapCountedPointer<CBankFileAsyncEntry>)
{
    return g_patchStatuses[g_patchUpdateCalls++];
}

static CMapCountedReference* MakeBankReference()
{
    CMapCountedReference* reference =
        static_cast<CMapCountedReference*>(operator new(sizeof(CMapCountedReference)));
    reference->Count = 1;
    reference->Release = static_cast<CMapCountedReleaseFunction>(&TestReleaseTarget::Release);
    reference->Owner = &g_releaseTarget;
    return reference;
}

static CEngineLandscapePatch* MakePatchArray(int count)
{
    int* allocation = static_cast<int*>(operator new(sizeof(int) + count * sizeof(CEngineLandscapePatch)));
    *allocation = count;
    CEngineLandscapePatch* patches = reinterpret_cast<CEngineLandscapePatch*>(allocation + 1);
    for (int index = 0; index < count; ++index)
        new (patches + index) CEngineLandscapePatch;
    return patches;
}

static void ResetCounters()
{
    g_beginCalls = g_discardCalls = g_asyncReleases = 0;
    g_ctorCalls = g_dtorCalls = g_initialiseCalls = g_loadHeaderCalls = 0;
    g_patchUpdateCalls = g_streamCtorCalls = g_streamDtorCalls = 0;
    g_lastOffset = g_lastReadSize = 0;
    g_lastStreamData = 0;
    g_lastStreamSize = 0;
    g_asyncFinished = false;
    g_asyncSize = sizeof(g_asyncBytes);
    for (int index = 0; index < 8; ++index)
    {
        g_initialiseX[index] = g_initialiseY[index] = -1;
        g_patchStatuses[index] = LS_LOADING_READY;
    }
}

static void ResetMap(
    CEngineLandscapeMap& map,
    CBackgroundTreeBounds& bounds,
    CBankFileAsyncEntry& bank)
{
    memset(&map, 0, sizeof(map));
    memset(&bounds, 0, sizeof(bounds));
    bounds.MinX = bounds.MinY = bounds.MinZ = -1.0f;
    bounds.MaxX = bounds.MaxY = bounds.MaxZ = 1.0f;
    map.BackgroundTree = &bounds;
    map.PatchColumns = 2;
    map.PatchRows = 1;
    map.BankEntry = &bank;
    map.BankEntryReference = MakeBankReference();
    map.ForegroundBlockOffset = 0x1234;
}

int main()
{
    fable_u8 camera[0x60];
    fable_u8 globals[0x620];
    memset(camera, 0, sizeof(camera));
    memset(globals, 0, sizeof(globals));
    DAT_01436ea0 = camera;
    DAT_01436ea8 = globals;
    DAT_013d2880 = 0;
    *reinterpret_cast<float*>(globals + 0x618) = 10.0f;

    CEngineLandscapeMap map;
    CBackgroundTreeBounds bounds;
    CBankFileAsyncEntry bank;

    ResetCounters();
    ResetMap(map, bounds, bank);
    map.BackgroundTree = 0;
    if (map.UpdateForeground() != LS_LOADING_READY || map.UpdateForegroundActive) return 1;

    ResetCounters();
    ResetMap(map, bounds, bank);
    if (map.UpdateForeground() != LS_LOADING_PENDING || map.UpdateForegroundActive) return 2;
    if (g_beginCalls != 1 || g_lastOffset != 0x1234 || g_lastReadSize != 0x48) return 3;
    if (map.ForegroundAsync.Pointer != &g_asyncData || g_asyncReference->Count != 1) return 4;
    if (map.UpdateForeground() != LS_LOADING_PENDING || g_beginCalls != 1) return 5;

    g_asyncFinished = true;
    if (map.UpdateForeground() != LS_LOADING_READY || map.UpdateForegroundActive) return 6;
    if (map.ForegroundPatches == 0 || map.ForegroundAsync.Pointer != 0) return 7;
    if (g_ctorCalls != 2 || g_initialiseCalls != 2 || g_loadHeaderCalls != 2) return 8;
    if (g_initialiseX[0] != 0 || g_initialiseY[0] != 0 ||
        g_initialiseX[1] != 16 || g_initialiseY[1] != 0) return 9;
    if (g_streamCtorCalls != 1 || g_streamDtorCalls != 1 ||
        g_lastStreamData != g_asyncBytes || g_lastStreamSize != sizeof(g_asyncBytes)) return 10;
    if (g_asyncReleases != 1 || g_patchUpdateCalls != 2 || map.BankEntryReference->Count != 1) return 11;

    g_patchUpdateCalls = 0;
    g_patchStatuses[0] = LS_LOADING_PENDING;
    g_patchStatuses[1] = LS_LOADING_READY;
    if (map.UpdateForeground() != LS_LOADING_PENDING || g_patchUpdateCalls != 2) return 12;
    g_patchUpdateCalls = 0;
    g_patchStatuses[0] = LS_LOADING_FAILED;
    if (map.UpdateForeground() != LS_LOADING_FAILED || g_patchUpdateCalls != 1) return 13;

    *reinterpret_cast<float*>(camera + 0x54) = 20.0f;
    map.KeepForegroundLoaded = false;
    if (map.UpdateForeground() != LS_LOADING_READY || map.ForegroundPatches != 0) return 14;
    if (g_dtorCalls != 2 || map.UpdateForegroundActive) return 15;

    ResetCounters();
    ResetMap(map, bounds, bank);
    map.ForegroundPatches = MakePatchArray(2);
    map.KeepForegroundLoaded = true;
    g_patchStatuses[0] = g_patchStatuses[1] = LS_LOADING_FAILED;
    if (map.UpdateForeground() != LS_LOADING_READY || map.ForegroundPatches == 0) return 16;
    if (g_patchUpdateCalls != 2 || g_dtorCalls != 0) return 17;
    map.KeepForegroundLoaded = false;
    if (map.UpdateForeground() != LS_LOADING_READY || map.ForegroundPatches != 0 || g_dtorCalls != 2) return 18;

    ResetCounters();
    ResetMap(map, bounds, bank);
    CMapCountedPointer<CBankFileAsyncData> async = bank.BeginReadDataAutoAlloc(1, 2);
    map.ForegroundAsync = async;
    async.Clear();
    if (map.UpdateForeground() != LS_LOADING_READY) return 19;
    if (g_discardCalls != 1 || map.ForegroundAsync.Pointer != 0 || g_asyncReleases != 1) return 20;

    printf("LANDSCAPE_MAP_UPDATE_FOREGROUND_TEST PASS\n");
    return 0;
}
