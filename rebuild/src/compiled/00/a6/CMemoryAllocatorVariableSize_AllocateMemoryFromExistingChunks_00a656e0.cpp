#include "fable_variable_size_allocator.h"
inline unsigned long CMemoryAllocatorBase::Align(unsigned long size) const
{
    return ((size+Alignment-1)>>AlignmentShift)<<AlignmentShift;
}
CMemoryAllocatorVariableSize::CMemoryArea*
CMemoryAllocatorVariableSize::AllocateMemoryFromExistingChunks(unsigned long size)
{
    size=Align(size);
    CMemoryArea* area=GetTreeAreaWithSizeGreaterThanOrEqualTo(size);
    if(area)return area->PParentChunk->AllocAreaFromArea(area,size);
    return 0;
}
