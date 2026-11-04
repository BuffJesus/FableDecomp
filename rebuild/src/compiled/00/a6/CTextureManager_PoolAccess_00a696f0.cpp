#include "engine/CTextureManager.h"
unsigned long CTextureManager::GetPoolCount(){return PoolCount;}
unsigned long CTextureManager::GetPoolTextureSize(unsigned long index){return PreallocSizes[index];}
unsigned long CTextureManager::GetPoolTextureCount(unsigned long index){return PreallocPools[index].Count;}
