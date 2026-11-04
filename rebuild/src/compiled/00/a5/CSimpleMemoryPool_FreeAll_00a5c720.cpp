#include "fable_simple_memory_pool.h"
void CSimpleMemoryPool::FreeAll()
{
 CMemoryBlock* block=FirstMemoryBlock;
 Root.NextFreeArea=0;
 if(block)
 {
  if(!block->NextBlock){CreateArea(block->BlockData,block->BlockSize);return;}
  ClearMemoryBlocks();
 }
}
