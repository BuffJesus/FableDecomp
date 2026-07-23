#ifndef FABLETLC_COMPOSED_TEXTURE_COMPILED_H
#define FABLETLC_COMPOSED_TEXTURE_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

class CTexture {};
class CGraphicDataBank {};

struct CSurfaceSpan
{
    fable_u8 pad_00[0x28];
    long* Begin;
    long* End;
};

struct CCompositeSurfaceInfo
{
    fable_u8 pad_00[4];
    CSurfaceSpan* SurfaceSpan;
};

template<class K, class V> class CKeyPairCompareLess {};

template<class K, class V, class Compare>
class CVectorMap
{
public:
    void* Begin;
    void* End;
    static void* __fastcall LowerBound(CVectorMap* map, K* key);
};

class CReleaseTarget2 {};
typedef void (CReleaseTarget2::*CReleaseMethod2)();

struct CGraphicFrameOwner
{
    long RefCount;
    CReleaseMethod2 Destroy;
    void* Object;
};

class CGraphicFrameData
{
public:
    long RefCount;
    fable_u8 pad_04[4];
    const CTexture* Texture;
    CGraphicFrameOwner* Owner;
    fable_u8 pad_10[0x0A];
    fable_u8 Flags;

    static void Uninitialise(CGraphicFrameData* data);
};

class CGraphicFrame
{
public:
    CGraphicFrameData* Data;
};

class CEngineSurfaceCompositionManager
{
public:
    fable_u8 pad_0000[0x10D4];
    fable_u8 CompositionEnabled;

    bool GetComposedSurface(
        CGraphicFrame* output,
        CCompositeSurfaceInfo* info,
        unsigned long appearanceId,
        bool useAlpha);
};

extern CEngineSurfaceCompositionManager* DAT_01436eac;

class CEnginePrimitiveManagerMeshBase
{
public:
    static const CTexture* __fastcall GetComposedTexture(
        long& composedTextureKey,
        CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<long, CCompositeSurfaceInfo> >& surfaceInfoMap,
        unsigned long appearanceId,
        bool useAlpha,
        const CGraphicDataBank* graphicDataBank);
};

FABLE_STATIC_ASSERT(offsetof(CSurfaceSpan, Begin) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CCompositeSurfaceInfo, SurfaceSpan) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CGraphicFrameData, Texture) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CGraphicFrameData, Owner) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CGraphicFrameData, Flags) == 0x1A);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, CompositionEnabled) == 0x10D4);

#endif
