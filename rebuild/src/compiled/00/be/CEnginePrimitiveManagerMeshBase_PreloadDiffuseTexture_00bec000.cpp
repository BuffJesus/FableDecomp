#include <new>
#include "candidates/preload_bump_texture_compiled.h"

static __forceinline void RefreshDiffuseResourceList(CResource* resource)
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

static __forceinline CGraphicInfoFrame* GetDiffuseInfoFrame(
    const CGraphicDataBank* bank,
    unsigned long index)
{
    unsigned long count = bank->EntriesEnd - bank->EntriesBegin;
    return index < count ? bank->EntriesBegin[index].InfoFrame : 0;
}

static bool TryPreloadDiffuseFrame(
    CGraphicInfoFrame* infoFrame,
    unsigned long appearanceId)
{
    if (infoFrame == 0)
        return false;

    unsigned long acceptableMipmap =
        CEnginePrimitiveManagerMeshBase::GetAcceptablePreloadMipmap(appearanceId, infoFrame);
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
        return true;
    }

    if (frameData->TextureHolder == 0)
    {
        frameData->BeginLoading(appearanceId, 0);
        return true;
    }

    long priority = infoFrame->Descriptor->LoadingContext->ResourcePriority;
    if (frameData->ResourcePriority != priority ||
        appearanceId <= static_cast<unsigned long>(frameData->RequestedMipmap))
    {
        frameData->ResourcePriority = priority;
        RefreshDiffuseResourceList(frameData->TextureResource);
        if (appearanceId <= static_cast<unsigned long>(frameData->CurrentPreloadMipmap))
            RefreshDiffuseResourceList(frameData);
        if (appearanceId < static_cast<unsigned long>(frameData->RequestedMipmap))
            frameData->RequestedMipmap = static_cast<signed char>(appearanceId);
    }

    if (appearanceId < static_cast<unsigned long>(frameData->CurrentPreloadMipmap))
        frameData->BeginLoading(appearanceId, 0);

    frameData = infoFrame->FrameData;
    unsigned long currentMipmap = frameData == 0 ?
        0xFFFFFFFFUL : static_cast<unsigned long>(frameData->CurrentPreloadMipmap);
    return acceptableMipmap < currentMipmap;
}

void __fastcall CEnginePrimitiveManagerMeshBase::PreloadDiffuseTexture(
    const C3DMeshMaterial* material,
    CEngineInternalPrimitiveMorphInfo* morphInfo,
    unsigned long appearanceId,
    bool& pendingLoad,
    const CGraphicDataBank* graphicDataBank)
{
    long textureId = material->DiffuseTextureId;
    if (textureId == 0)
        return;

    long composedTextureKey = textureId;
    if (morphInfo != 0)
    {
        CTextureSurfaceEntry* entry = morphInfo->TextureSurfaces.Begin;
        CTextureSurfaceEntry* end = morphInfo->TextureSurfaces.End;
        while (entry != end &&
               (entry->TextureId < textureId ||
                (entry->TextureId == textureId && entry->SubIndex < material->DiffuseSubIndex)))
            ++entry;

        if (entry != end && entry->TextureId == textureId &&
            entry->SubIndex == material->DiffuseSubIndex)
        {
            for (CTextureSurfaceIndex* surface = entry->SurfaceBegin;
                 surface != entry->SurfaceEnd; ++surface)
            {
                if (TryPreloadDiffuseFrame(
                        GetDiffuseInfoFrame(graphicDataBank, surface->GraphicIndex), appearanceId))
                    pendingLoad = true;
            }
        }

        if (GetComposedTexture(
                composedTextureKey,
                morphInfo->ComposedSurfaceInfoMap,
                appearanceId,
                false,
                graphicDataBank) != 0)
            return;
    }

    if (TryPreloadDiffuseFrame(
            GetDiffuseInfoFrame(graphicDataBank, composedTextureKey), appearanceId))
        pendingLoad = true;
}
