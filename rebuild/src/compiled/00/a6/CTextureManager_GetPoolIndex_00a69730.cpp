#include "engine/CTextureManager.h"
long CTextureManager::GetPoolIndex(unsigned long size)
{
    long remaining=PoolCount;
    long first=0;
    while(remaining>0) {
        long half=remaining/2;
        long middle=first+half;
        unsigned long value=PreallocSizes[middle];
        if(size==value)return middle;
        if(size>value) {
            first=middle+1;
            remaining-=half+1;
        } else remaining=half;
    }
    if(static_cast<unsigned long>(first)<PoolCount)return first;
    return -1;
}
