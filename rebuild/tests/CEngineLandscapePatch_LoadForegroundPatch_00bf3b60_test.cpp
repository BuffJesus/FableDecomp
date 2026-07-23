#include <new>
#include <stdio.h>
#include <string.h>
#include "candidates/landscape_patch_load_foreground_compiled.h"

static CBankFileAsyncData g_asyncData;
static fable_u8 g_compressed[128];
static bool g_finished;
static int g_beginCalls, g_asyncReleases, g_bankReleases;
static unsigned long g_beginOffset, g_beginSize;
static int g_createLayerCalls, g_destroyLayerCalls, g_loadLayerCalls;
static int g_failCreateLayerAt, g_failLoadLayerAt;
static int g_allocateCalls;
static bool g_failAllocation, g_failDecompression, g_failWater;
static unsigned long g_allocationBytes;
static int g_createWaterCalls, g_destroyWaterCalls;
static fable_u32 g_waterX, g_waterY;
static CLandscapeLayerMesh g_layers[4];
static CWaterPatchMesh g_water;

static void ReleaseAsync(void*) { ++g_asyncReleases; }
static void ReleaseBank(void*) { ++g_bankReleases; }

bool CBankFileAsyncData::IsFinished() { return g_finished; }
const fable_u8* CBankFileAsyncData::GetData() { return g_compressed; }

CForegroundCountedPointer<CBankFileAsyncData> CBankFileAsyncEntry::BeginReadDataAutoAlloc(
    unsigned long offset,
    unsigned long size)
{
    ++g_beginCalls;
    g_beginOffset = offset;
    g_beginSize = size;
    CForegroundCountedReference* reference =
        static_cast<CForegroundCountedReference*>(operator new(sizeof(CForegroundCountedReference)));
    reference->Count = 0;
    reference->Release = ReleaseAsync;
    reference->Owner = 0;
    return CForegroundCountedPointer<CBankFileAsyncData>(&g_asyncData, reference);
}

CLandscapeLayerMesh* CreateLandscapeLayerMesh(CEngineLandscapeMap*)
{
    int index = g_createLayerCalls++;
    if (index == g_failCreateLayerAt)
        return 0;
    memset(&g_layers[index], 0, sizeof(g_layers[index]));
    return &g_layers[index];
}

void DestroyLandscapeLayerMesh(CLandscapeLayerMesh*) { ++g_destroyLayerCalls; }

bool CLandscapeLayerMesh::LoadForeground(CMemoryDataInputStream&, CEngineTexturePalette&)
{
    int index = g_loadLayerCalls++;
    if (index == g_failLoadLayerAt)
        return false;
    ForegroundTextureIndex = index == 0 ? 5 : 0;
    BumpMapTextureIndex = index == 1 ? 7 : 0;
    return true;
}

void* AllocateLandscapePatchTextureIDs(unsigned long bytes, CEngineLandscapePatch*)
{
    ++g_allocateCalls;
    g_allocationBytes = bytes;
    if (g_failAllocation)
        return 0;
    return operator new(bytes == 0 ? 1 : bytes);
}

bool DecompressLandscapeForeground(
    const fable_u8* source,
    unsigned long sourceSize,
    fable_u8* destination,
    unsigned long* destinationSize)
{
    if (g_failDecompression || *destinationSize < sourceSize)
        return false;
    for (unsigned long index = 0; index < sourceSize; ++index)
        destination[index] = source[index];
    *destinationSize = sourceSize;
    return true;
}

CWaterPatchMesh* CreateWaterPatchMesh(
    CEngineLandscapeMap*,
    CMemoryDataInputStream&,
    fable_u32 x,
    fable_u32 y)
{
    ++g_createWaterCalls;
    g_waterX = x;
    g_waterY = y;
    return g_failWater ? 0 : &g_water;
}

void DestroyWaterPatchMesh(CWaterPatchMesh*) { ++g_destroyWaterCalls; }

static void Put16(fable_u8*& output, fable_u16 value)
{
    *output++ = static_cast<fable_u8>(value);
    *output++ = static_cast<fable_u8>(value >> 8);
}

static void Put32(fable_u8*& output, fable_u32 value)
{
    Put16(output, static_cast<fable_u16>(value));
    Put16(output, static_cast<fable_u16>(value >> 16));
}

static int BuildCompressed(fable_u16 layers, bool water)
{
    fable_u8 payload[64];
    fable_u8* write = payload;
    Put16(write, layers);
    *write++ = water ? 1 : 0;
    if (water)
    {
        Put32(write, 0x11223344);
        Put32(write, 0x55667788);
    }
    unsigned long payloadSize = static_cast<unsigned long>(write - payload);
    write = g_compressed;
    Put32(write, payloadSize);
    Put32(write, payloadSize);
    memcpy(write, payload, payloadSize);
    return static_cast<int>(payloadSize + 8);
}

static CForegroundCountedReference* MakeBankReference()
{
    CForegroundCountedReference* reference =
        static_cast<CForegroundCountedReference*>(operator new(sizeof(CForegroundCountedReference)));
    reference->Count = 0;
    reference->Release = ReleaseBank;
    reference->Owner = 0;
    return reference;
}

static void Reset(CEngineLandscapePatch& patch, CEngineLandscapeMap& map)
{
    memset(&patch, 0, sizeof(patch));
    memset(&map, 0, sizeof(map));
    patch.Map = &map;
    patch.FilePosition = 0x12345678;
    patch.ForegroundDataSize = BuildCompressed(2, false);
    g_finished = false;
    g_beginCalls = g_asyncReleases = g_bankReleases = 0;
    g_createLayerCalls = g_destroyLayerCalls = g_loadLayerCalls = 0;
    g_failCreateLayerAt = g_failLoadLayerAt = -1;
    g_allocateCalls = 0;
    g_failAllocation = g_failDecompression = g_failWater = false;
    g_allocationBytes = 0;
    g_createWaterCalls = g_destroyWaterCalls = 0;
    g_waterX = g_waterY = 0;
}

int main()
{
    CEngineLandscapePatch patch;
    CEngineLandscapeMap map;
    CBankFileAsyncEntry bank;

    Reset(patch, map);
    patch.LayerMeshList = &g_layers[0];
    CForegroundCountedReference* bankRef = MakeBankReference();
    if (patch.LoadForegroundPatch(CForegroundCountedPointer<CBankFileAsyncEntry>(&bank, bankRef)) != LS_LOADING_READY) return 1;
    if (g_beginCalls != 0 || g_bankReleases != 1) return 2;

    Reset(patch, map);
    bankRef = MakeBankReference();
    if (patch.LoadForegroundPatch(CForegroundCountedPointer<CBankFileAsyncEntry>(&bank, bankRef)) != LS_LOADING_PENDING) return 3;
    if (g_beginCalls != 1 || g_beginOffset != 0x12345678 ||
        g_beginSize != static_cast<unsigned long>(patch.ForegroundDataSize)) return 4;
    if (patch.LoadingPatch.Pointer != &g_asyncData || g_asyncReleases != 0) return 5;

    g_finished = true;
    bankRef = MakeBankReference();
    if (patch.LoadForegroundPatch(CForegroundCountedPointer<CBankFileAsyncEntry>(&bank, bankRef)) != LS_LOADING_READY) return 6;
    if (patch.LayerMeshList != &g_layers[0] || g_layers[0].Next != &g_layers[1]) return 7;
    if (patch.FastAccessTextureIDCount != 2 || patch.FastAccessTextureIDs[0] != 5 ||
        patch.FastAccessTextureIDs[1] != 7 || g_allocationBytes != 8) return 8;
    if ((patch.Flags & 2) == 0 || patch.LoadingPatch.Pointer != 0 || g_asyncReleases != 1) return 9;
    operator delete(patch.FastAccessTextureIDs);

    Reset(patch, map);
    patch.ForegroundDataSize = BuildCompressed(0, true);
    patch.WaterPatch = &g_water;
    bankRef = MakeBankReference();
    if (patch.LoadForegroundPatch(CForegroundCountedPointer<CBankFileAsyncEntry>(&bank, bankRef)) != LS_LOADING_PENDING) return 10;
    g_finished = true;
    bankRef = MakeBankReference();
    if (patch.LoadForegroundPatch(CForegroundCountedPointer<CBankFileAsyncEntry>(&bank, bankRef)) != LS_LOADING_READY) return 11;
    if (g_destroyWaterCalls != 1 || g_createWaterCalls != 1 || patch.WaterPatch != &g_water) return 12;
    if (g_waterX != 0x11223344 || g_waterY != 0x55667788) return 13;
    operator delete(patch.FastAccessTextureIDs);

    Reset(patch, map);
    g_failLoadLayerAt = 0;
    bankRef = MakeBankReference();
    if (patch.LoadForegroundPatch(CForegroundCountedPointer<CBankFileAsyncEntry>(&bank, bankRef)) != LS_LOADING_PENDING) return 14;
    g_finished = true;
    bankRef = MakeBankReference();
    if (patch.LoadForegroundPatch(CForegroundCountedPointer<CBankFileAsyncEntry>(&bank, bankRef)) != LS_LOADING_FAILED) return 15;
    if (g_destroyLayerCalls != 1 || (patch.Flags & 2) != 0 || patch.LoadingPatch.Pointer == 0) return 16;
    patch.LoadingPatch.Clear();

    Reset(patch, map);
    g_failAllocation = true;
    bankRef = MakeBankReference();
    if (patch.LoadForegroundPatch(CForegroundCountedPointer<CBankFileAsyncEntry>(&bank, bankRef)) != LS_LOADING_PENDING) return 17;
    g_finished = true;
    bankRef = MakeBankReference();
    if (patch.LoadForegroundPatch(CForegroundCountedPointer<CBankFileAsyncEntry>(&bank, bankRef)) != LS_LOADING_FAILED) return 18;
    if (g_allocateCalls != 1 || patch.LoadingPatch.Pointer == 0) return 19;
    patch.LoadingPatch.Clear();

    printf("LANDSCAPE_PATCH_LOAD_FOREGROUND_TEST PASS\n");
    return 0;
}
