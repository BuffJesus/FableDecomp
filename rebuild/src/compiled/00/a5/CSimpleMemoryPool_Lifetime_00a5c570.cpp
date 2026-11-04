#include "fable_simple_memory_pool.h"
CSimpleMemoryPool::CSimpleMemoryPool(unsigned long capacity,unsigned long growth):GrowthGranularity(Align(growth)) {
    Root.Size=0;Root.NextFreeArea=0;FirstMemoryBlock=0;Capacity=0x400;
    if(capacity)Capacity=Align(capacity);
}
#pragma inline_depth(0)
CSimpleMemoryPool::~CSimpleMemoryPool(){ClearMemoryBlocks();}
