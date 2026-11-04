#ifndef FABLE_ENGINE_CENGINERESOURCEMANAGER_H
#define FABLE_ENGINE_CENGINERESOURCEMANAGER_H
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_counted_pointer.h"
class CCPPointerInfo;
class CMovableResourceMemoryPool;
class CGraphicDataBank;
class CMeshDataBank;
class CTexture;
class C2DExtentsI;
class CPixelFormat;
class CWideString;
// Partial RETAIL layout. Ego_r embedded bank/layout sizes differ substantially.
// Do not use sizeof(this view) to allocate a resource manager. Unknown spans
// remain opaque. Getter instructions establish the fields and anchor below.
#pragma pack(push, 1)
class CEngineResourceManager {
public:
    CTexture* AllocateExternalTextureFromResourcePools(unsigned long,const C2DExtentsI&,long,const CPixelFormat&);
    void FreeExternalTextureFromResourcePools(CTexture*);
    unsigned long SetupEditorAnimation(const CWideString&);
    bool ResizeHiresTextureMemory(long);
    unsigned char UnrecoveredPrefix[0x10];
    CMeshDataBank* MeshBanks[3]; // static, editor animation, engine banks; donor array and retail +10/+14.
    unsigned char UnrecoveredBeforeLoadingPool[0x8e0-0x1c];
    CCountedPointer<CMovableResourceMemoryPool> LoadingMemoryPool;
    unsigned char UnrecoveredLoadingFailureHandler[8];
    CMovableResourceMemoryPool* AnimationMemoryPool_Data;
    CCPPointerInfo* AnimationMemoryPool_Info;
    unsigned char UnrecoveredResourcesBeforeGraphics[0x954-0x8f8];
    CGraphicDataBank* GraphicBanks;
    CGraphicDataBank* MainGraphicBank;
    unsigned char UnrecoveredResources[0x3190-0x95c];
    unsigned char GPUScratchBufferTextureManager[1]; // Anchor only; size not inferred.
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager,LoadingMemoryPool)==0x8e0);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager,MeshBanks)==0x10);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager,LoadingMemoryPool)+4==0x8e4);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager,AnimationMemoryPool_Data)==0x8f0);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager,AnimationMemoryPool_Info)==0x8f4);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager,GraphicBanks)==0x954);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager,MainGraphicBank)==0x958);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager,GPUScratchBufferTextureManager)==0x3190);
#endif
