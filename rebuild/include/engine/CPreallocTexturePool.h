#pragma once
#include <stddef.h>
#include "fable_game_text_bank.h"
#include "fable_carray.h"
class CManagedTextureData;
class CPreallocTexturePool : public CResourceList {
public:
    CManagedTextureData* Allocate();
    void Initialise(unsigned long poolIndex,unsigned long maxSize,unsigned long count);
    void Uninitialise();
    unsigned long GetTexturesAllocated();
    unsigned long GetTexturesInUse(unsigned long unusedFrames);
    void FrameEnd();
    unsigned long GetTextureCount();
    bool ResizePool(unsigned long);
    unsigned long Count;
    unsigned long MaxSize;
    long PoolIndex;
    CArray<CIVCountedPointer<CManagedTextureData> > Textures;
};
FABLE_STATIC_ASSERT(offsetof(CPreallocTexturePool,Count)==0x3c);
FABLE_STATIC_ASSERT(offsetof(CPreallocTexturePool,Textures)==0x48);
FABLE_STATIC_ASSERT(sizeof(CPreallocTexturePool)==0x54);
