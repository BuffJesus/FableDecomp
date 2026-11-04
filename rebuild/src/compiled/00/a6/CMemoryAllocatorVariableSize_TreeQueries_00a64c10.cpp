#include "fable_variable_size_allocator.h"

CMemoryAllocatorVariableSize::CMemoryArea*
CMemoryAllocatorVariableSize::GetTreeAreaWithSizeGreaterThanOrEqualTo(long size) const
{
    CMemoryArea* area=PTreeRoot;
    CMemoryArea* best=0;
    while(area) {
        if(area->DataSize==size)return area;
        if(area->DataSize>size) {
            if(!best || area->DataSize<best->DataSize)best=area;
            area=area->PLeftChild;
        } else area=area->PRightChild;
    }
    return best;
}

long CMemoryAllocatorVariableSize::GetLargestTreeAreaSize() const
{
    CMemoryArea* area=PTreeRoot;
    long size=0;
    while(area) {
        size=area->DataSize;
        area=area->PRightChild;
    }
    return size;
}

long CMemoryAllocatorVariableSize::GetSmallestTreeAreaSize() const
{
    CMemoryArea* area=PTreeRoot;
    long size=0;
    while(area) {
        size=area->DataSize;
        area=area->PLeftChild;
    }
    return size;
}
