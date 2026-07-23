#include <new>
#include "candidates/composed_texture_compiled.h"

struct CompositeMapEntry
{
    long Key;
    CCompositeSurfaceInfo Value;
};

static __forceinline void ReleaseGraphicFrameData(CGraphicFrameData* data)
{
    --data->RefCount;
    if (data->RefCount == 0)
    {
        if ((data->Flags & 0x10) != 0)
            CGraphicFrameData::Uninitialise(data);
        CGraphicFrameOwner* owner = data->Owner;
        if (owner != 0)
        {
            --owner->RefCount;
            if (owner->RefCount == 0)
            {
                (static_cast<CReleaseTarget2*>(owner->Object)->*owner->Destroy)();
                operator delete(owner);
            }
        }
        data->Texture = 0;
        data->Owner = 0;
        operator delete(data);
    }
}

const CTexture* __fastcall CEnginePrimitiveManagerMeshBase::GetComposedTexture(
    long& composedTextureKey,
    CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<long, CCompositeSurfaceInfo> >& surfaceInfoMap,
    unsigned long appearanceId,
    bool useAlpha,
    const CGraphicDataBank*)
{
    typedef CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<long, CCompositeSurfaceInfo> > SurfaceMap;
    CGraphicFrameData* composedFrameData;
    CompositeMapEntry* it = reinterpret_cast<CompositeMapEntry*>(
        SurfaceMap::LowerBound(&surfaceInfoMap, &composedTextureKey));
    CompositeMapEntry* end = reinterpret_cast<CompositeMapEntry*>(surfaceInfoMap.End);
    if (it == end || composedTextureKey < it->Key)
        it = end;

    if (it != end)
    {
        CCompositeSurfaceInfo* info = &it->Value;
        long spanBytes = reinterpret_cast<long>(info->SurfaceSpan->End) -
            reinterpret_cast<long>(info->SurfaceSpan->Begin);
        if (((spanBytes >> 4) > 1) &&
            DAT_01436eac->CompositionEnabled != 0)
        {
            composedFrameData = 0;
            bool composed = DAT_01436eac->GetComposedSurface(
                reinterpret_cast<CGraphicFrame*>(&composedFrameData),
                info,
                appearanceId,
                useAlpha);
            CGraphicFrameData* returnedFrameData = composedFrameData;
            if (composed)
            {
                const CTexture* texture = composedFrameData->Texture;
                ReleaseGraphicFrameData(returnedFrameData);
                return texture;
            }
            if (composedFrameData != 0)
                ReleaseGraphicFrameData(returnedFrameData);
        }
        composedTextureKey = *info->SurfaceSpan->Begin;
    }
    return 0;
}
