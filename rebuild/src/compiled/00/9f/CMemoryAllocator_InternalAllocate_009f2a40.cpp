#include "fable_memory_allocator.h"
void* CMemoryAllocator::InternalAllocate(long size,unsigned long category,bool debug)
{
    if(size<1)size=1;
    long requestedSize=size;
    size+=20;
    unsigned long index=size;
    if(index>static_cast<unsigned long>(MaxFixedAllocationSizes[category]+1))index=MaxFixedAllocationSizes[category]+1;
    CAllocSizeToAllocator* lookup=&AllocSizeToAllocator[category][index];
    unsigned char* allocation=static_cast<unsigned char*>(lookup->PAllocator->Allocate(size));
    if(allocation){
        unsigned long address=reinterpret_cast<unsigned long>(allocation);
        if(address<g_FableAllocatorRangeStart_013CAAA4||address>=g_FableAllocatorRangeEnd_013CAAA8)allocation[-1]=lookup->ID;
        ++NoAllocations;
        allocation+=4;
        InitDebugInfo(allocation,debug,requestedSize);
    }
    return allocation;
}
