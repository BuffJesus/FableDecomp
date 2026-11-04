#include "engine/CTextureManager.h"
bool CTextureManager::ResizePoolTextureCount(unsigned long size,unsigned long count)
{
    bool result=false;
    long index=GetPoolIndex(size);
    if(static_cast<unsigned long>(index)<PoolCount) {
        FrameEnd();
        result=PreallocPools[index].ResizePool(count);
    }
    return result;
}
