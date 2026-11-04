#include "fable_variable_size_allocator.h"

CMemoryAllocatorVariableSize::CMemoryAreaChunk*
CMemoryAllocatorVariableSize::NewChunkDemandPaged(void* memory,long size)
{
    CMemoryAreaChunk* chunk=static_cast<CMemoryAreaChunk*>(memory);
    long dataSize=size-(((g_FableVariableAllocatorAlignment_013D28D4+sizeof(CMemoryAreaChunk)-1)>>
          g_FableVariableAllocatorAlignmentShift_013D28D8)<<g_FableVariableAllocatorAlignmentShift_013D28D8);
    chunk->DemandPaged=true;
    chunk->Init(static_cast<unsigned char*>(memory)+
        (((g_FableVariableAllocatorAlignment_013D28D4+sizeof(CMemoryAreaChunk)-1)>>
          g_FableVariableAllocatorAlignmentShift_013D28D8)<<g_FableVariableAllocatorAlignmentShift_013D28D8),
        dataSize,this);
    chunk->PNextChunk=PFirstChunk;
    if(PFirstChunk)PFirstChunk->PPreviousChunk=chunk;
    chunk->PPreviousChunk=0;
    PFirstChunk=chunk;
    return chunk;
}
