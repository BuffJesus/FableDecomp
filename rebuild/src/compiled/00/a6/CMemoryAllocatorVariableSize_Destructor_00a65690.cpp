#include "fable_variable_size_allocator.h"
inline void CMemoryAllocatorBase::SetCurrentAlignment()
{
    g_FableVariableAllocatorAlignmentShift_013D28D8=AlignmentShift;
    g_FableVariableAllocatorAlignment_013D28D4=Alignment;
}
CMemoryAllocatorVariableSize::~CMemoryAllocatorVariableSize()
{
    SetCurrentAlignment();
    while(PFirstChunk)DeleteChunk(PFirstChunk);
}
