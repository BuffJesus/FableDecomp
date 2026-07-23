#include <new>
#include "candidates/preload_bump_texture_compiled.h"

static __forceinline void RefreshResourceList(CResource* resource)
{
    if (resource != 0 && resource->ResourceList != 0 &&
        resource->ListGeneration != resource->ResourceList->Generation)
    {
        resource->ListRemove();
        resource->ListInsert(
            resource->ResourceList->Head,
            reinterpret_cast<unsigned char*>(resource->ResourceList) + 4);
        resource->ListGeneration = resource->ResourceList->Generation;
    }
}

void __fastcall CEnginePrimitiveManagerMeshBase::PreloadBumpTexture(
    const C3DMeshMaterial* material,
    CEngineInternalPrimitiveMorphInfo* morphInfo,
    unsigned long appearanceId,
    bool& pendingLoad,
    const CGraphicDataBank* graphicDataBank)
{
    long textureKey = material->BumpTextureId;
    if (textureKey == 0)
        return;

    if (morphInfo != 0)
    {
        if (GetComposedTexture(
                textureKey,
                morphInfo->ComposedSurfaceInfoMap,
                appearanceId,
                true,
                graphicDataBank) != 0)
            return;
    }

    unsigned long entryCount = graphicDataBank->EntriesEnd - graphicDataBank->EntriesBegin;
    if (static_cast<unsigned long>(textureKey) >= entryCount)
        return;

    const CGraphicDataBankEntry* entry = graphicDataBank->EntriesBegin + textureKey;
    if (entry == 0 || entry->InfoFrame == 0)
        return;

    CGraphicInfoFrame* infoFrame = entry->InfoFrame;
    unsigned long acceptableMipmap = GetAcceptablePreloadMipmap(appearanceId, infoFrame);
    CFrameData* frameData = infoFrame->FrameData;

    if (frameData == 0)
    {
        frameData = new CFrameData(infoFrame);
        if (infoFrame->FrameData != 0)
        {
            --infoFrame->FrameData->RefCount;
            if (infoFrame->FrameData->RefCount == 0)
                infoFrame->FrameData->Release();
            infoFrame->FrameData = 0;
        }
        infoFrame->FrameData = frameData;
        frameData->BeginLoading(appearanceId, 0);
        pendingLoad = true;
        return;
    }

    if (frameData->TextureHolder == 0)
    {
        frameData->BeginLoading(appearanceId, 0);
        pendingLoad = true;
        return;
    }

    long resourcePriority = infoFrame->Descriptor->LoadingContext->ResourcePriority;
    if (frameData->ResourcePriority != resourcePriority ||
        appearanceId <= static_cast<unsigned long>(frameData->RequestedMipmap))
    {
        frameData->ResourcePriority = resourcePriority;
        RefreshResourceList(frameData->TextureResource);

        if (appearanceId <= static_cast<unsigned long>(frameData->CurrentPreloadMipmap))
            RefreshResourceList(frameData);

        if (appearanceId < static_cast<unsigned long>(frameData->RequestedMipmap))
            frameData->RequestedMipmap = static_cast<signed char>(appearanceId);
    }

    if (appearanceId < static_cast<unsigned long>(frameData->CurrentPreloadMipmap))
        frameData->BeginLoading(appearanceId, 0);

    frameData = infoFrame->FrameData;
    unsigned long currentMipmap = frameData == 0 ?
        0xFFFFFFFFUL : static_cast<unsigned long>(frameData->CurrentPreloadMipmap);
    if (acceptableMipmap < currentMipmap)
        pendingLoad = true;
}
