#include <new>
#include "candidates/preload_bump_texture_compiled.h"

static __forceinline void RefreshOverlayResourceList(CResource* resource)
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

bool __fastcall CEnginePrimitiveManagerMeshBase::GetOverlaidTextures(
    const CEngineTextureDecal& textureDecal,
    CArray<const CTexture*>& overlaidTextures,
    unsigned long appearanceId,
    const CGraphicDataBank* graphicDataBank)
{
    unsigned long textureCount = textureDecal.EntriesEnd - textureDecal.EntriesBegin;
    const CTexture* initialTexture = 0;
    overlaidTextures.resize(textureCount, initialTexture);

    bool allTexturesAvailable = true;
    for (unsigned long index = 0; index < textureCount; ++index)
    {
        fable_u32 textureId = textureDecal.EntriesBegin[index].TextureId;
        unsigned long bankEntryCount = graphicDataBank->EntriesEnd - graphicDataBank->EntriesBegin;
        if (textureId >= bankEntryCount)
            continue;

        const CGraphicDataBankEntry* entry = graphicDataBank->EntriesBegin + textureId;
        if (entry == 0 || entry->InfoFrame == 0)
            continue;

        CGraphicInfoFrame* infoFrame = entry->InfoFrame;
        CFrameData* frameData = infoFrame->FrameData;
        const CTexture** output = &overlaidTextures[index];

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
        }
        else if (frameData->TextureHolder != 0)
        {
            long resourcePriority = infoFrame->Descriptor->LoadingContext->ResourcePriority;
            if (frameData->ResourcePriority != resourcePriority ||
                appearanceId <= static_cast<unsigned long>(frameData->RequestedMipmap))
            {
                frameData->ResourcePriority = resourcePriority;
                RefreshOverlayResourceList(frameData->TextureResource);

                if (appearanceId <= static_cast<unsigned long>(frameData->CurrentPreloadMipmap))
                    RefreshOverlayResourceList(frameData);

                if (appearanceId < static_cast<unsigned long>(frameData->RequestedMipmap))
                    frameData->RequestedMipmap = static_cast<signed char>(appearanceId);
            }

            if (appearanceId < static_cast<unsigned long>(frameData->CurrentPreloadMipmap))
            {
                frameData->BeginLoading(appearanceId, 0);
                *output = infoFrame->FrameData->TextureHolder->Texture;
            }
            else
            {
                *output = frameData->TextureHolder->Texture;
            }
            continue;
        }

        frameData->BeginLoading(appearanceId, 0);
        allTexturesAvailable = false;
    }

    return allTexturesAvailable;
}
