#ifndef FABLETLC_PRELOAD_BUMP_TEXTURE_COMPILED_H
#define FABLETLC_PRELOAD_BUMP_TEXTURE_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

class CTexture {};

struct CSurfaceInfoMap
{
    void* Begin;
    void* End;
};

class C3DMeshMaterial
{
public:
    fable_u8 Unknown00[0x0C];
    fable_i32 DiffuseSubIndex;
    fable_u32 DiffuseTextureId;
    fable_u32 BumpTextureId;
};

struct CTextureSurfaceIndex
{
    fable_u32 GraphicIndex;
    fable_u8 Unknown04[0x0C];
};

struct CTextureSurfaceEntry
{
    fable_i32 TextureId;
    fable_i32 SubIndex;
    fable_u8 Unknown08[8];
    CTextureSurfaceIndex* SurfaceBegin;
    CTextureSurfaceIndex* SurfaceEnd;
};

struct CTextureSurfaceMap
{
    CTextureSurfaceEntry* Begin;
    CTextureSurfaceEntry* End;
};

class CEngineInternalPrimitiveMorphInfo
{
public:
    CTextureSurfaceMap TextureSurfaces;
    fable_u8 Unknown08[8];
    CSurfaceInfoMap ComposedSurfaceInfoMap;
};

class CResource;

class CResourceList
{
public:
    fable_u8 Unknown00[0x10];
    CResource* Head;
    fable_u8 Unknown14[0x18];
    long Generation;
};

class CResource
{
public:
    virtual void Slot00();
    virtual void Release();

    long RefCount;
    CResourceList* ResourceList;
    fable_u8 Unknown0C[0x0C];
    long ListGeneration;

    void ListRemove();
    void ListInsert(CResource* previous, void* next);
};

class CGraphicInfoFrame;

class CTextureHolder
{
public:
    fable_u8 Unknown00[8];
    const CTexture* Texture;
};

class CFrameData : public CResource
{
public:
    CTextureHolder* TextureHolder;
    fable_u32 Unknown20;
    CResource* TextureResource;
    fable_u8 Unknown28[0x10];
    signed char CurrentPreloadMipmap;
    fable_u8 Unknown39;
    signed char RequestedMipmap;
    fable_u8 Unknown3B;
    long ResourcePriority;
    fable_u8 Unknown40[4];

    explicit CFrameData(CGraphicInfoFrame* owner);
    void BeginLoading(unsigned long appearanceId, long flags);
};

class CGraphicLoadingContext
{
public:
    fable_u8 Unknown000[0x194];
    long ResourcePriority;
};

class CGraphicFrameDescriptor
{
public:
    fable_u8 Unknown00[0x20];
    CGraphicLoadingContext* LoadingContext;
};

class CGraphicInfoFrame
{
public:
    fable_u8 Unknown00[8];
    CFrameData* FrameData;
    CGraphicFrameDescriptor* Descriptor;
};

struct CGraphicDataBankEntry
{
    fable_u8 Unknown00[0x28];
    CGraphicInfoFrame* InfoFrame;
};

class CGraphicDataBank
{
public:
    fable_u8 Unknown000[0x1E0];
    CGraphicDataBankEntry* EntriesBegin;
    CGraphicDataBankEntry* EntriesEnd;
};

struct CEngineTextureDecalEntry
{
    fable_u32 TextureId;
    fable_u8 Unknown04[0x0C];
};

class CEngineTextureDecal
{
public:
    fable_u8 Unknown00[8];
    CEngineTextureDecalEntry* EntriesBegin;
    CEngineTextureDecalEntry* EntriesEnd;
};

template<class T>
class CArray
{
public:
    T* Data;
    unsigned long Size;
    unsigned long Capacity;

    void resize(unsigned long size, const T& initialValue);
    T& operator[](unsigned long index) { return Data[index]; }
};

class CEnginePrimitiveManagerMeshBase
{
public:
    static const CTexture* __fastcall GetComposedTexture(
        long& composedTextureKey,
        CSurfaceInfoMap& surfaceInfoMap,
        unsigned long appearanceId,
        bool useAlpha,
        const CGraphicDataBank* graphicDataBank);

    static unsigned long __fastcall GetAcceptablePreloadMipmap(
        unsigned long appearanceId,
        CGraphicInfoFrame* infoFrame);

    static void __fastcall PreloadBumpTexture(
        const C3DMeshMaterial* material,
        CEngineInternalPrimitiveMorphInfo* morphInfo,
        unsigned long appearanceId,
        bool& pendingLoad,
        const CGraphicDataBank* graphicDataBank);

    static void __fastcall PreloadDiffuseTexture(
        const C3DMeshMaterial* material,
        CEngineInternalPrimitiveMorphInfo* morphInfo,
        unsigned long appearanceId,
        bool& pendingLoad,
        const CGraphicDataBank* graphicDataBank);

    static bool __fastcall GetOverlaidTextures(
        const CEngineTextureDecal& textureDecal,
        CArray<const CTexture*>& overlaidTextures,
        unsigned long appearanceId,
        const CGraphicDataBank* graphicDataBank);
};

FABLE_STATIC_ASSERT(offsetof(C3DMeshMaterial, DiffuseSubIndex) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(C3DMeshMaterial, DiffuseTextureId) == 0x10);
FABLE_STATIC_ASSERT(offsetof(C3DMeshMaterial, BumpTextureId) == 0x14);
FABLE_STATIC_ASSERT(sizeof(CTextureSurfaceIndex) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CTextureSurfaceEntry, SurfaceBegin) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CTextureSurfaceEntry, SurfaceEnd) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveMorphInfo, ComposedSurfaceInfoMap) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CResource, RefCount) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CResource, ResourceList) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CResource, ListGeneration) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CTextureHolder, Texture) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CFrameData, TextureHolder) == 0x1C);
FABLE_STATIC_ASSERT(offsetof(CFrameData, TextureResource) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CFrameData, CurrentPreloadMipmap) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CFrameData, RequestedMipmap) == 0x3A);
FABLE_STATIC_ASSERT(offsetof(CFrameData, ResourcePriority) == 0x3C);
FABLE_STATIC_ASSERT(sizeof(CFrameData) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CGraphicInfoFrame, FrameData) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CGraphicInfoFrame, Descriptor) == 0x0C);
FABLE_STATIC_ASSERT(sizeof(CGraphicDataBankEntry) == 0x2C);
FABLE_STATIC_ASSERT(offsetof(CGraphicDataBankEntry, InfoFrame) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CGraphicDataBank, EntriesBegin) == 0x1E0);
FABLE_STATIC_ASSERT(offsetof(CGraphicDataBank, EntriesEnd) == 0x1E4);
FABLE_STATIC_ASSERT(sizeof(CEngineTextureDecalEntry) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CEngineTextureDecal, EntriesBegin) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CEngineTextureDecal, EntriesEnd) == 0x0C);

#endif
