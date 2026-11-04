#include "candidates/fixed_size_allocator_accessors_compiled.h"
void CMemoryAllocatorFixedSize::RemoveChunkFromList(CMemoryAreaChunk* chunk)
{
    if(chunk->PNextChunk)chunk->PNextChunk->PPreviousChunk=chunk->PPreviousChunk;
    if(chunk->PPreviousChunk)chunk->PPreviousChunk->PNextChunk=chunk->PNextChunk;
    if(PFirstChunk==chunk)PFirstChunk=chunk->PNextChunk;
    if(PLastChunk==chunk)PLastChunk=chunk->PPreviousChunk;
}
void CMemoryAllocatorFixedSize::InsertChunkAtListHead(CMemoryAreaChunk* chunk)
{
    chunk->PNextChunk=PFirstChunk;
    if(PFirstChunk)PFirstChunk->PPreviousChunk=chunk;
    chunk->PPreviousChunk=0;
    CMemoryAreaChunk* previousHead=PFirstChunk;
    PFirstChunk=chunk;
    if(!previousHead)PLastChunk=chunk;
}
void CMemoryAllocatorFixedSize::InsertChunkAtListTail(CMemoryAreaChunk* chunk)
{
    chunk->PPreviousChunk=PLastChunk;
    if(PLastChunk)PLastChunk->PNextChunk=chunk;
    chunk->PNextChunk=0;
    CMemoryAreaChunk* previousTail=PLastChunk;
    PLastChunk=chunk;
    if(!previousTail)PFirstChunk=chunk;
}
