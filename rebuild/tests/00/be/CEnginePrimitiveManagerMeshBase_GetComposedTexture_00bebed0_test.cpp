#include <new>
#include <stdio.h>
#include <string.h>
#include "candidates/composed_texture_compiled.h"

typedef CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<long, CCompositeSurfaceInfo> > SurfaceMap;
static void* g_lowerBoundResult;
static bool g_composeResult;
static CGraphicFrameData* g_returnedFrame;
static int g_composeCalls;
static int g_uninitialiseCalls;

template<>
void* __fastcall SurfaceMap::LowerBound(SurfaceMap*, long*)
{
    return g_lowerBoundResult;
}

CEngineSurfaceCompositionManager* DAT_01436eac;

void CGraphicFrameData::Uninitialise(CGraphicFrameData*) { ++g_uninitialiseCalls; }

bool CEngineSurfaceCompositionManager::GetComposedSurface(
    CGraphicFrame* output,
    CCompositeSurfaceInfo*,
    unsigned long,
    bool)
{
    ++g_composeCalls;
    output->Data = g_returnedFrame;
    return g_composeResult;
}

struct TestMapEntry
{
    long Key;
    CCompositeSurfaceInfo Value;
};

static CGraphicFrameData* MakeFrame(const CTexture* texture, unsigned char flags)
{
    CGraphicFrameData* frame = static_cast<CGraphicFrameData*>(operator new(sizeof(CGraphicFrameData)));
    memset(frame, 0, sizeof(*frame));
    frame->RefCount = 1;
    frame->Texture = texture;
    frame->Flags = flags;
    return frame;
}

int main()
{
    CEngineSurfaceCompositionManager compositionManager;
    SurfaceMap map;
    TestMapEntry entry;
    CSurfaceSpan span;
    long surfaces[8] = { 77 };
    CTexture texture;
    memset(&compositionManager, 0, sizeof(compositionManager));
    memset(&map, 0, sizeof(map));
    memset(&entry, 0, sizeof(entry));
    memset(&span, 0, sizeof(span));
    DAT_01436eac = &compositionManager;
    entry.Key = 5;
    entry.Value.SurfaceSpan = &span;
    span.Begin = surfaces;
    span.End = surfaces + 8;
    map.End = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(&entry) + sizeof(entry));
    g_uninitialiseCalls = g_composeCalls = 0;

    long key = 5;
    g_lowerBoundResult = map.End;
    if (CEnginePrimitiveManagerMeshBase::GetComposedTexture(key, map, 1, false, 0) != 0) return 1;

    g_lowerBoundResult = &entry;
    compositionManager.CompositionEnabled = 0;
    key = 5;
    if (CEnginePrimitiveManagerMeshBase::GetComposedTexture(key, map, 1, false, 0) != 0 || key != 77) return 2;

    compositionManager.CompositionEnabled = 1;
    g_composeResult = true;
    g_returnedFrame = MakeFrame(&texture, 0);
    key = 5;
    if (CEnginePrimitiveManagerMeshBase::GetComposedTexture(key, map, 2, true, 0) != &texture) return 3;
    if (g_composeCalls != 1) return 4;

    g_composeResult = false;
    g_returnedFrame = MakeFrame(&texture, 0x10);
    key = 5;
    if (CEnginePrimitiveManagerMeshBase::GetComposedTexture(key, map, 3, false, 0) != 0 || key != 77) return 5;
    if (g_composeCalls != 2 || g_uninitialiseCalls != 1) return 6;
    printf("COMPOSED_TEXTURE_TEST PASS\n");
    return 0;
}
