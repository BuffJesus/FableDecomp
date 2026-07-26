#include <new>
#include <stdio.h>
#include <string.h>
#include "candidates/landscape_background_patch_compressed.h"

static fable_u8 g_memoryRoot[0x700];
static fable_u8 g_memoryManager[0x20];
fable_u8* DAT_01436ea8 = g_memoryRoot;
fable_u8 DAT_013cb498;

static unsigned long g_overflowValues[2];
static int g_overflowIndex;
static int g_setPositionCalls;
static long g_setPosition;
static int g_decompressCalls;
static fable_i32 g_decompressSource;
static fable_i32 g_decompressSize;
static unsigned long g_decompressCapacity;
static void* g_expectedPool;
static void* g_seenPool;
static void* g_seenTag;
static unsigned long g_allocationSize;
static unsigned long g_constructedStreamSize;
static unsigned char* g_constructedStreamData;
static float g_worldX, g_worldY;
static bool g_loadResult;

void CMemoryDataInputStream::Slot00() {}
void CMemoryDataInputStream::Slot04() {}
long CMemoryDataInputStream::GetPosition() { return ReadPosition; }

CMemoryDataInputStream::CMemoryDataInputStream(unsigned char* data, unsigned long size)
{
    ReadPosition = 0;
    Unknown08 = 0;
    Cursor = data;
    Unknown10 = 0;
    RemainingInSourceChunk = size;
    SourceChunkBase = 0;
    g_constructedStreamData = data;
    g_constructedStreamSize = size;
}

CMemoryDataInputStream::~CMemoryDataInputStream() {}

void CMemoryDataInputStream::ReadWithSrcChunkOverflow(
    CMemoryDataInputStream* stream,
    unsigned char* destination,
    unsigned long size)
{
    if (size == 4)
    {
        *reinterpret_cast<unsigned long*>(destination) = g_overflowValues[g_overflowIndex++];
        stream->ReadPosition += 4;
    }
}

void CMemoryDataInputStream::SetPositionAssumeWithinSourceChunk(long position)
{
    ++g_setPositionCalls;
    g_setPosition = position;
    ReadPosition = position;
}

CTemporaryBuffer::CTemporaryBuffer(unsigned long size, void* pool, void* tag)
{
    Unknown00 = 0;
    Data = new unsigned char[size];
    g_allocationSize = size;
    g_seenPool = pool;
    g_seenTag = tag;
}

CTemporaryBuffer::~CTemporaryBuffer() { delete[] Data; }

void LZSS_Decompress(
    fable_i32 source,
    fable_i32 compressedSize,
    unsigned char* destination,
    unsigned long* destinationSize,
    fable_i32 flags)
{
    ++g_decompressCalls;
    g_decompressSource = source;
    g_decompressSize = compressedSize;
    g_decompressCapacity = *destinationSize;
    if (flags == 0 && destination != 0 && *destinationSize != 0)
        memset(destination, 0x5A, *destinationSize);
}

bool CLandscapeBackgroundPatch::Load(
    CMemoryDataInputStream&,
    float worldX,
    float worldY)
{
    g_worldX = worldX;
    g_worldY = worldY;
    return g_loadResult;
}

static void Reset()
{
    memset(g_memoryRoot, 0, sizeof(g_memoryRoot));
    memset(g_memoryManager, 0, sizeof(g_memoryManager));
    *reinterpret_cast<void**>(g_memoryRoot + 0x6B0) = g_memoryManager;
    g_expectedPool = g_memoryRoot;
    *reinterpret_cast<void**>(g_memoryManager + 8) = g_expectedPool;
    g_overflowIndex = g_setPositionCalls = g_decompressCalls = 0;
    g_setPosition = 0;
    g_decompressSource = g_decompressSize = 0;
    g_decompressCapacity = g_allocationSize = 0;
    g_seenPool = g_seenTag = 0;
    g_constructedStreamSize = 0;
    g_constructedStreamData = 0;
    g_worldX = g_worldY = 0.0f;
    g_loadResult = true;
}

int main()
{
    CLandscapeBackgroundPatch patch;

    Reset();
    unsigned long words[3] = { 8, 4, 0x11223344 };
    CMemoryDataInputStream direct(reinterpret_cast<unsigned char*>(words), sizeof(words));
    direct.SourceChunkBase = 100;
    if (!patch.LoadCompressed(direct, 1.25f, -2.5f)) return 1;
    if (g_decompressCalls != 1 || g_decompressSource != 108 || g_decompressSize != 4) return 2;
    if (g_decompressCapacity != 11 || g_allocationSize != 11) return 3;
    if (g_setPositionCalls != 1 || g_setPosition != 12) return 4;
    if (g_constructedStreamSize != 8 || g_constructedStreamData == 0) return 5;
    if (g_seenPool != g_expectedPool || g_seenTag != &DAT_013cb498) return 6;
    if (g_worldX != 1.25f || g_worldY != -2.5f) return 7;

    Reset();
    unsigned char empty[1] = { 0 };
    CMemoryDataInputStream overflow(empty, 0);
    g_overflowValues[0] = 5;
    g_overflowValues[1] = 0;
    g_loadResult = false;
    if (patch.LoadCompressed(overflow, 3.0f, 4.0f)) return 8;
    if (g_overflowIndex != 2 || g_decompressSource != 0 || g_decompressSize != 0) return 9;
    if (g_decompressCapacity != 8 || g_allocationSize != 8) return 10;
    if (g_setPositionCalls != 0 || g_constructedStreamSize != 5) return 11;

    printf("LANDSCAPE_BACKGROUND_PATCH_COMPRESSED_TEST PASS\n");
    return 0;
}
