#ifndef FABLE_ENGINE_CTEXTUREMANAGER_H
#define FABLE_ENGINE_CTEXTUREMANAGER_H
// Retail reconciled: each preallocation pool is54bytes,donor58bytes.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTextureManager = 0x614; retail = 0x5d4.
// Retail evidence: 1 trusted landed function(s); reconciliation 1 conflict(s) in 1 file(s) vs 0 agreeing; PINNED by 2 retyped file(s) despite conflicts.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CPreallocTexturePool.h"
class CPixelFormat;
class CGraphicFrame;
class CManagedTexture;
class CManagedTextureOwner;

#pragma pack(push, 1)
class CTextureManager {
public:
    static unsigned long FABLE_FASTCALL CalculateTextureSize(unsigned long,unsigned long,bool,const CPixelFormat&);
    bool ResizePoolTextureCount(unsigned long,unsigned long);
    unsigned long GetPoolTexturesAllocated(unsigned long poolIndex);
    unsigned long GetPoolTexturesInUse(unsigned long poolIndex,unsigned long unusedFrames);
    unsigned long GetPoolCount();
    unsigned long GetPoolTextureSize(unsigned long poolIndex);
    unsigned long GetPoolTextureCount(unsigned long poolIndex);
    bool AllocateTexture(CManagedTexture&,unsigned long,unsigned long,unsigned int,const CPixelFormat&,CManagedTextureOwner*);
    bool AllocateVolumeTexture(CManagedTexture&,unsigned long,unsigned long,unsigned long,unsigned int,const CPixelFormat&,CManagedTextureOwner*);
    bool AllocateBestAvailableTexture(CManagedTexture&,unsigned long,unsigned long,unsigned int,const CPixelFormat&,unsigned long,unsigned long,unsigned long*,CManagedTextureOwner*);
    void FrameEnd();
    void Uninitialise();
    bool ReduceMipmapLevel(CGraphicFrame&,CManagedTexture&,unsigned long);
private:
    static bool ReuseBusyTextures;
    long GetPoolIndex(unsigned long);
public:
    static bool FABLE_FASTCALL GetReuseBusyTextures() {return ReuseBusyTextures;}
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    bool          Initialised;                               // +0x04
    unsigned char _pad_0x05[0x3];                            // +0x05
    unsigned long PoolCount;                                 // +0x08
    unsigned long PreallocSizes[16];                         // +0x0c
    CPreallocTexturePool PreallocPools[16];                      // +0x4c CPreallocTexturePool[16]
    unsigned long FailedAllocations[16];                     // +0x58c
    long          BufferSize;                                // +0x5cc
    void*         TextureBuffer;                             // +0x5d0
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CTextureManager) == 0x5d4);
FABLE_STATIC_ASSERT(offsetof(CTextureManager, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTextureManager, Initialised) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CTextureManager, PoolCount) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CTextureManager, PreallocSizes) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTextureManager, PreallocPools) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CTextureManager, FailedAllocations) == 0x58c);
FABLE_STATIC_ASSERT(offsetof(CTextureManager, BufferSize) == 0x5cc);
FABLE_STATIC_ASSERT(offsetof(CTextureManager, TextureBuffer) == 0x5d0);

#endif // FABLE_ENGINE_CTEXTUREMANAGER_H
