#include <stdio.h>
#include <string.h>
#include "candidates/preload_bump_texture_compiled.h"

static int g_constructCalls;
static int g_beginCalls;
static int g_removeCalls;
static int g_insertCalls;
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

template<>
void CArray<const CTexture*>::resize(unsigned long size, const CTexture* const& initialValue)
{
    delete[] Data;
    Data = size != 0 ? new const CTexture*[size] : 0;
    Size = Capacity = size;
    for (unsigned long i = 0; i < size; ++i)
        Data[i] = initialValue;
}

static void Reset()
{
    g_constructCalls = g_beginCalls = g_removeCalls = g_insertCalls = 0;
    g_beginAppearance = 0;
}

int main()
{
    CEngineTextureDecal decal;
    CEngineTextureDecalEntry decalEntries[3];
    CArray<const CTexture*> output;
    CGraphicDataBank bank;
    CGraphicDataBankEntry bankEntries[3];
    CGraphicInfoFrame frame;
    CGraphicFrameDescriptor descriptor;
    CGraphicLoadingContext loadingContext;
    CTexture texture;
    CTextureHolder holder;

    memset(&decal, 0, sizeof(decal));
    memset(decalEntries, 0, sizeof(decalEntries));
    memset(&output, 0, sizeof(output));
    memset(&bank, 0, sizeof(bank));
    memset(bankEntries, 0, sizeof(bankEntries));
    memset(&frame, 0, sizeof(frame));
    memset(&descriptor, 0, sizeof(descriptor));
    memset(&loadingContext, 0, sizeof(loadingContext));
    memset(&holder, 0, sizeof(holder));
    decal.EntriesBegin = decalEntries;
    bank.EntriesBegin = bankEntries;
    bank.EntriesEnd = bankEntries + 3;
    frame.Descriptor = &descriptor;
    descriptor.LoadingContext = &loadingContext;
    holder.Texture = &texture;

    Reset();
    decal.EntriesEnd = decalEntries;
    if (!CEnginePrimitiveManagerMeshBase::GetOverlaidTextures(decal, output, 2, &bank)) return 1;
    if (output.Size != 0 || g_beginCalls != 0) return 2;

    Reset();
    decal.EntriesEnd = decalEntries + 2;
    decalEntries[0].TextureId = 9;
    decalEntries[1].TextureId = 2;
    bankEntries[2].InfoFrame = 0;
    if (!CEnginePrimitiveManagerMeshBase::GetOverlaidTextures(decal, output, 2, &bank)) return 3;
    if (output.Size != 2 || output[0] != 0 || output[1] != 0) return 4;

    Reset();
    decal.EntriesEnd = decalEntries + 1;
    decalEntries[0].TextureId = 1;
    bankEntries[1].InfoFrame = &frame;
    frame.FrameData = 0;
    if (CEnginePrimitiveManagerMeshBase::GetOverlaidTextures(decal, output, 3, &bank)) return 5;
    if (g_constructCalls != 1 || g_beginCalls != 1 || g_beginAppearance != 3) return 6;
    delete frame.FrameData;

    Reset();
    CFrameData loaded(&frame);
    loaded.TextureHolder = &holder;
    loaded.CurrentPreloadMipmap = 4;
    loaded.RequestedMipmap = 1;
    loaded.ResourcePriority = 7;
    loadingContext.ResourcePriority = 7;
    frame.FrameData = &loaded;
    if (!CEnginePrimitiveManagerMeshBase::GetOverlaidTextures(decal, output, 2, &bank)) return 7;
    if (output[0] != &texture || g_beginCalls != 1 || g_beginAppearance != 2) return 8;

    printf("GET_OVERLAID_TEXTURES_TEST PASS\n");
    delete[] output.Data;
    return 0;
}
