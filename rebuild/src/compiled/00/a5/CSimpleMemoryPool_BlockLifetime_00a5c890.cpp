#include "fable_simple_memory_pool.h"
#include "fable_memory_allocator.h"
CSimpleMemoryPool::CMemoryBlock::CMemoryBlock(unsigned long size)
: BlockSize(size),NextBlock(0)
{
 BlockData=CMemoryAllocator::AllocateAligned(size,16);
 OwnsData=true;
}
CSimpleMemoryPool::CMemoryBlock::~CMemoryBlock()
{
 if(OwnsData)CMemoryAllocator::DeallocateAligned(BlockData);
}
void CSimpleMemoryPool::CreateNewMemoryBlock(unsigned long size)
{
 CMemoryBlock* block=new CMemoryBlock(size+16);
 CreateArea(block->BlockData,block->BlockSize);
 if(FirstMemoryBlock)Capacity+=size;
 block->NextBlock=FirstMemoryBlock;
 FirstMemoryBlock=block;
}
