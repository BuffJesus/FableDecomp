#include "fable_simple_memory_pool.h"
unsigned long CSimpleMemoryPool::CalculateMemoryAllocatedByPool() const
{
 unsigned long total=0;
 for(CMemoryBlock* block=FirstMemoryBlock;block;block=block->NextBlock)total+=block->BlockSize;
 return total;
}
