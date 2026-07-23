#include <stdio.h>
#include <string.h>
#include "candidates/preload_bump_texture_compiled.h"

static const CTexture g_texture;
static bool g_composed;
static int g_composedCalls, g_constructCalls, g_beginCalls;
static int g_removeCalls, g_insertCalls;
static unsigned long g_acceptableMipmap;

void CResource::Slot00() {}
void CResource::Release() { delete this; }
void CResource::ListRemove() { ++g_removeCalls; }
void CResource::ListInsert(CResource*, void*) { ++g_insertCalls; }

CFrameData::CFrameData(CGraphicInfoFrame*)
{
    ++g_constructCalls;
    RefCount = 1; ResourceList = 0; ListGeneration = 0;
    TextureHolder = 0; TextureResource = 0;
    CurrentPreloadMipmap = RequestedMipmap = 127;
    ResourcePriority = 0;
}

void CFrameData::BeginLoading(unsigned long, long) { ++g_beginCalls; }

const CTexture* __fastcall CEnginePrimitiveManagerMeshBase::GetComposedTexture(
    long&, CSurfaceInfoMap&, unsigned long, bool useAlpha, const CGraphicDataBank*)
{
    ++g_composedCalls;
    if (useAlpha) return 0;
    return g_composed ? &g_texture : 0;
}

unsigned long __fastcall CEnginePrimitiveManagerMeshBase::GetAcceptablePreloadMipmap(
    unsigned long, CGraphicInfoFrame*)
{
    return g_acceptableMipmap;
}

static void Reset()
{
    g_composed = false;
    g_composedCalls = g_constructCalls = g_beginCalls = 0;
    g_removeCalls = g_insertCalls = 0;
    g_acceptableMipmap = 0;
}

int main()
{
    C3DMeshMaterial material;
    CEngineInternalPrimitiveMorphInfo morph;
    CGraphicDataBank bank;
    CGraphicDataBankEntry bankEntries[4];
    CGraphicInfoFrame frames[4];
    CGraphicFrameDescriptor descriptors[4];
    CGraphicLoadingContext contexts[4];
    CTextureSurfaceEntry surfaceEntry;
    CTextureSurfaceIndex surfaceIndices[2];
    memset(&material, 0, sizeof(material));
    memset(&morph, 0, sizeof(morph));
    memset(&bank, 0, sizeof(bank));
    memset(bankEntries, 0, sizeof(bankEntries));
    memset(frames, 0, sizeof(frames));
    memset(descriptors, 0, sizeof(descriptors));
    memset(contexts, 0, sizeof(contexts));
    memset(&surfaceEntry, 0, sizeof(surfaceEntry));
    memset(surfaceIndices, 0, sizeof(surfaceIndices));
    bank.EntriesBegin = bankEntries;
    bank.EntriesEnd = bankEntries + 4;
    for (int i = 0; i < 4; ++i)
    {
        bankEntries[i].InfoFrame = &frames[i];
        frames[i].Descriptor = &descriptors[i];
        descriptors[i].LoadingContext = &contexts[i];
    }

    Reset();
    bool pending = false;
    CEnginePrimitiveManagerMeshBase::PreloadDiffuseTexture(&material, &morph, 2, pending, &bank);
    if (g_composedCalls != 0 || pending) return 1;

    Reset();
    material.DiffuseTextureId = 1;
    material.DiffuseSubIndex = 3;
    g_composed = true;
    CEnginePrimitiveManagerMeshBase::PreloadDiffuseTexture(&material, &morph, 2, pending, &bank);
    if (g_composedCalls != 1 || g_constructCalls != 0 || pending) return 2;

    Reset(); pending = false; g_composed = true;
    surfaceEntry.TextureId = 1; surfaceEntry.SubIndex = 3;
    surfaceIndices[0].GraphicIndex = 2; surfaceIndices[1].GraphicIndex = 3;
    surfaceEntry.SurfaceBegin = surfaceIndices; surfaceEntry.SurfaceEnd = surfaceIndices + 2;
    morph.TextureSurfaces.Begin = &surfaceEntry; morph.TextureSurfaces.End = &surfaceEntry + 1;
    CEnginePrimitiveManagerMeshBase::PreloadDiffuseTexture(&material, &morph, 2, pending, &bank);
    if (!pending || g_constructCalls != 2 || g_beginCalls != 2) return 3;
    delete frames[2].FrameData; delete frames[3].FrameData;
    frames[2].FrameData = frames[3].FrameData = 0;

    Reset(); pending = false; g_composed = false;
    morph.TextureSurfaces.Begin = morph.TextureSurfaces.End = 0;
    CEnginePrimitiveManagerMeshBase::PreloadDiffuseTexture(&material, 0, 2, pending, &bank);
    if (!pending || g_constructCalls != 1 || g_beginCalls != 1) return 4;
    delete frames[1].FrameData;

    printf("PRELOAD_DIFFUSE_TEXTURE_TEST PASS\n");
    return 0;
}
