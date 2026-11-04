#include "engine/CTextureManager.h"
#include "fable_managed_texture_data.h"
unsigned long CTextureManager::GetPoolTexturesAllocated(unsigned long index) {
    CPreallocTexturePool& pool=PreallocPools[index];
    unsigned long allocated=0;
    for(unsigned long i=0;i<pool.Count;++i)
        if(pool.Textures[i]->Allocated) ++allocated;
    return allocated;
}
unsigned long CTextureManager::GetPoolTexturesInUse(unsigned long index,unsigned long unusedFrames) {
    return PreallocPools[index].GetTexturesInUse(unusedFrames);
}
