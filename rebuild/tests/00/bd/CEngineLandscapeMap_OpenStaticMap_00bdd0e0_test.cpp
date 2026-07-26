#include <stdio.h>
#include <string.h>
#include "candidates/landscape_map_open_compiled.h"

static int g_paletteLoads;
static int g_overflowCalls;
static fable_u32 g_overflowWords[5];

void CDataInputStream::ReadWithSrcChunkOverflow(CDataInputStream*, unsigned char* output, unsigned long bytes)
{
    ++g_overflowCalls;
    memcpy(output, g_overflowWords, bytes);
}
void CEngineTexturePalette::Load(CDataInputStream&) { ++g_paletteLoads; }

class MapTestStream : public CDataInputStream
{
public:
    unsigned long selectedChunk;
    virtual void Slot00() {}
    virtual void SetSrcChunk(unsigned long chunk) { selectedChunk = chunk; }
};

int main()
{
    CEngineLandscapeMap map;
    CEngineStaticMapFileBlockManagerRef manager;
    CRefCountBlock oldRef;
    CRefCountBlock newRef;
    MapTestStream stream;
    fable_u32 words[5] = { 0x1234U, 13U, 2U, 3U, 4U };
    memset(&map, 0, sizeof(map));
    memset(&oldRef, 0, sizeof(oldRef));
    memset(&newRef, 0, sizeof(newRef));
    oldRef.RefCount = 2;
    newRef.RefCount = 5;
    manager.Object = &manager;
    manager.RefCountBlock = &newRef;
    map.BlockManagerRefCountBlock = &oldRef;
    stream.ReadPosition = 7;
    stream.SrcCursor32 = words;
    stream.SrcBytesRemaining = 20;
    stream.selectedChunk = 0;
    g_paletteLoads = 0;
    g_overflowCalls = 0;

    map.OpenStaticMap(&manager, stream);
    if (map.StaticMapFileBlockManager != &manager) return 1;
    if (map.BlockManagerObject != manager.Object || map.BlockManagerRefCountBlock != &newRef) return 2;
    if (oldRef.RefCount != 1 || newRef.RefCount != 6) return 3;
    if (stream.selectedChunk != words[0] || g_paletteLoads != 1) return 4;
    if (stream.SrcCursor32 != words + 5 || stream.SrcBytesRemaining != 0 || stream.ReadPosition != 27) return 5;

    g_overflowWords[0] = 0x99887766U;
    g_overflowWords[1] = 0;
    map.OpenStaticMap(&manager, stream);
    if (newRef.RefCount != 6 || g_paletteLoads != 2) return 6;
    if (g_overflowCalls != 1 || stream.selectedChunk != g_overflowWords[0]) return 7;
    printf("LANDSCAPE_MAP_OPEN_STATIC_MAP_TEST PASS\n");
    return 0;
}
