#include <stdio.h>
#include <string.h>
#include "candidates/local_detail_cache_compiled.h"

static int g_overflowCalls;
static int g_quadCalls;
static int g_paletteCalls;
static fable_u32 g_overflowWords[3];

void CDataInputStream::ReadWithSrcChunkOverflow(CDataInputStream*, unsigned char* output, unsigned long bytes)
{
    ++g_overflowCalls;
    memcpy(output, g_overflowWords, bytes);
}

void CQuadTreeElement::LoadHeader(CDataInputStream&) { ++g_quadCalls; }
void CObjectTypeCollectionPalette::Load(CDataInputStream&) { ++g_paletteCalls; }

class TestStream : public CDataInputStream
{
public:
    unsigned long selectedChunk;
    virtual void Slot00() {}
    virtual void SetSrcChunk(unsigned long chunk) { selectedChunk = chunk; }
};

static int RunBuffered()
{
    CLocalDetailCacheMap map;
    CEngineStaticMapFileBlockManager manager;
    TestStream stream;
    fable_u32 words[3] = { 0x11223344U, 0x55U, 0x66U };
    memset(&map, 0, sizeof(map));
    stream.ReadPosition = 10;
    stream.SrcCursor32 = words;
    stream.SrcBytesRemaining = 12;
    stream.selectedChunk = 0;
    map.OpenStaticMap(&manager, stream);
    if (map.StaticMapFileBlockManager != &manager) return 1;
    if (stream.selectedChunk != words[0]) return 2;
    if (stream.SrcCursor32 != words + 3 || stream.SrcBytesRemaining != 0 || stream.ReadPosition != 22) return 3;
    return 0;
}

int main()
{
    g_overflowCalls = g_quadCalls = g_paletteCalls = 0;
    int result = RunBuffered();
    if (result != 0) return result;

    CLocalDetailCacheMap map;
    CEngineStaticMapFileBlockManager manager;
    TestStream stream;
    memset(&map, 0, sizeof(map));
    stream.ReadPosition = 0;
    stream.SrcCursor32 = 0;
    stream.SrcBytesRemaining = 0;
    stream.selectedChunk = 0;
    g_overflowWords[0] = 0xAABBCCDDU;
    g_overflowWords[1] = 1;
    g_overflowWords[2] = 2;
    map.OpenStaticMap(&manager, stream);
    if (g_overflowCalls != 1 || stream.selectedChunk != g_overflowWords[0]) return 10;
    if (g_quadCalls != 2 || g_paletteCalls != 2) return 11;
    printf("LOCAL_DETAIL_OPEN_STATIC_MAP_TEST PASS\n");
    return 0;
}
