#include "fable_variable_size_allocator.h"
inline void CMemoryAllocatorBase::SetCurrentAlignment()
{
    g_FableVariableAllocatorAlignmentShift_013D28D8=AlignmentShift;
    g_FableVariableAllocatorAlignment_013D28D4=Alignment;
}
unsigned long CMemoryAllocatorVariableSize::GetNoFreeAreas()
{
    SetCurrentAlignment();
    unsigned long count=0;
    for(CMemoryAreaChunk* chunk=PFirstChunk;chunk;chunk=chunk->PNextChunk)
        count+=chunk->NoFreeAreas;
    return count;
}
unsigned long CMemoryAllocatorVariableSize::GetNoAllocatedAreas()
{
    SetCurrentAlignment();
    for(CMemoryAreaChunk* chunk=PFirstChunk;chunk;chunk=chunk->PNextChunk){}
    return 0;
}
