#include "fable_variable_size_allocator.h"
inline void CMemoryAllocatorBase::SetCurrentAlignment()
{
    g_FableVariableAllocatorAlignmentShift_013D28D8=AlignmentShift;
    g_FableVariableAllocatorAlignment_013D28D4=Alignment;
}
float CMemoryAllocatorVariableSize::GetFragmentation()
{
    SetCurrentAlignment();
    unsigned long free=GetNoFreeAreas();
    unsigned long allocated=GetNoAllocatedAreas();
    if(free<=1)return 0.0f;
    return static_cast<float>(free-1)/static_cast<float>(allocated);
}
