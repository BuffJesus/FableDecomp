#include <stdio.h>
#include <string.h>
#include "candidates/preload_bump_texture_compiled.h"

static const CTexture g_texture;
static bool g_composed;
static int g_composedCalls;
static int g_constructCalls;
static int g_beginCalls;
static int g_removeCalls;
static int g_insertCalls;
static unsigned long g_acceptableMipmap;
static unsigned long g_beginAppearance;

void CResource::Slot00() {}
void CResource::Release() { delete this; }
void CResource::ListRemove() { ++g_removeCalls; }
void CResource::ListInsert(CResource*, void*) { ++g_insertCalls; }

CFrameData::CFrameData(CGraphicInfoFrame*)
{
    ++g_constructCalls;
    RefCount = 1;
    ResourceList = 0;
    ListGeneration = 0;
    TextureHolder = 0;
    TextureResource = 0;
    CurrentPreloadMipmap = 127;
    RequestedMipmap = 127;
    ResourcePriority = 0;
}

void CFrameData::BeginLoading(unsigned long appearanceId, long)
{
    ++g_beginCalls;
    g_beginAppearance = appearanceId;
}

const CTexture* __fastcall CEnginePrimitiveManagerMeshBase::GetComposedTexture(
    long&,
    CSurfaceInfoMap&,
    unsigned long,
    bool,
    const CGraphicDataBank*)
{
    ++g_composedCalls;
    return g_composed ? &g_texture : 0;
}

unsigned long __fastcall CEnginePrimitiveManagerMeshBase::GetAcceptablePreloadMipmap(
    unsigned long,
    CGraphicInfoFrame*)
{
    return g_acceptableMipmap;
}

static void Reset()
{
    g_composed = false;
    g_composedCalls = g_constructCalls = g_beginCalls = 0;
    g_removeCalls = g_insertCalls = 0;
    g_acceptableMipmap = 0;
    g_beginAppearance = 0;
}

int main()
{
    C3DMeshMaterial material;
    CEngineInternalPrimitiveMorphInfo morph;
    CGraphicDataBank bank;
    CGraphicDataBankEntry entries[2];
    CGraphicInfoFrame frame;
    CGraphicFrameDescriptor descriptor;
    CGraphicLoadingContext loadingContext;

    memset(&material, 0, sizeof(material));
    memset(&morph, 0, sizeof(morph));
    memset(&bank, 0, sizeof(bank));
    memset(entries, 0, sizeof(entries));
    memset(&frame, 0, sizeof(frame));
    memset(&descriptor, 0, sizeof(descriptor));
    memset(&loadingContext, 0, sizeof(loadingContext));
    bank.EntriesBegin = entries;
    bank.EntriesEnd = entries + 2;
    entries[1].InfoFrame = &frame;
    frame.Descriptor = &descriptor;
    descriptor.LoadingContext = &loadingContext;

    Reset();
    bool pending = false;
    CEnginePrimitiveManagerMeshBase::PreloadBumpTexture(&material, &morph, 3, pending, &bank);
    if (g_composedCalls != 0 || pending) return 1;

    Reset(); material.BumpTextureId = 1; g_composed = true;
    CEnginePrimitiveManagerMeshBase::PreloadBumpTexture(&material, &morph, 3, pending, &bank);
    if (g_composedCalls != 1 || g_constructCalls != 0 || pending) return 2;

    Reset(); pending = false; frame.FrameData = 0;
    CEnginePrimitiveManagerMeshBase::PreloadBumpTexture(&material, 0, 3, pending, &bank);
    if (!pending || g_constructCalls != 1 || g_beginCalls != 1 || g_beginAppearance != 3) return 3;
    delete frame.FrameData;

    Reset(); pending = false;
    CFrameData unloaded(&frame);
    unloaded.TextureHolder = 0;
    frame.FrameData = &unloaded;
    CEnginePrimitiveManagerMeshBase::PreloadBumpTexture(&material, 0, 4, pending, &bank);
    if (!pending || g_beginCalls != 1 || g_beginAppearance != 4) return 4;

    Reset(); pending = false;
    CFrameData loaded(&frame);
    CTextureHolder holder;
    loaded.TextureHolder = &holder;
    loaded.CurrentPreloadMipmap = 4;
    loaded.RequestedMipmap = 1;
    loaded.ResourcePriority = 7;
    loadingContext.ResourcePriority = 7;
    frame.FrameData = &loaded;
    g_acceptableMipmap = 3;
    CEnginePrimitiveManagerMeshBase::PreloadBumpTexture(&material, 0, 2, pending, &bank);
    if (!pending || g_beginCalls != 1 || g_beginAppearance != 2) return 5;
    if (g_removeCalls != 0 || g_insertCalls != 0) return 6;

    printf("PRELOAD_BUMP_TEXTURE_TEST PASS\n");
    return 0;
}
