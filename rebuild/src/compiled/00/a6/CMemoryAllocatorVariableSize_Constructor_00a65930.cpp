#include "fable_variable_size_allocator.h"

CMemoryAllocatorVariableSize::CMemoryAllocatorVariableSize(void*,
    long minimumChunkSize,void* initialMemory,long initialSize,
    unsigned long alignment,unsigned long flags,unsigned long internalFlags)
    : CMemoryAllocatorBase(alignment),PFirstChunk(0)
{
    MinChunkSize=minimumChunkSize;
    Flags=flags;
    InternalFlags=internalFlags;
    PTreeRoot=0;
    g_FableVariableAllocatorAlignmentShift_013D28D8=AlignmentShift;
    g_FableVariableAllocatorAlignment_013D28D4=Alignment;
    if(initialSize!=0) {
        if(initialMemory!=0)NewChunkDemandPaged(initialMemory,initialSize)->Permanent=true;
        else NewChunk(initialSize)->Permanent=true;
    }
}
