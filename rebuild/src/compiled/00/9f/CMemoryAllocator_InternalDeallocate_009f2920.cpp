#include "fable_memory_allocator.h"
void CMemoryAllocator::InternalDeallocate(void* allocation)
{
    unsigned char* area=static_cast<unsigned char*>(allocation)-4;
    unsigned long address=reinterpret_cast<unsigned long>(allocation);
    unsigned char id;
    if(address>=g_FableAllocatorRangeStart_013CAAA4&&address<g_FableAllocatorRangeEnd_013CAAA8)
        id=static_cast<unsigned char>((address-g_FableAllocatorRangeStart_013CAAA4)>>24);
    else id=*(static_cast<unsigned char*>(allocation)-5);
    if(id<NoAllocators){
        CMemoryAllocatorBase* allocator=AllocatorInfo[id].PAllocator;
        GetAllocationSize(allocation);
        unsigned long* metadata=reinterpret_cast<unsigned long*>(static_cast<unsigned char*>(allocation)+GetAllocationSize(allocation)-20);
        MemoryAllocatedPerTrackingID[metadata[1]]-=metadata[2];
        memset(area,0xcd,allocator->GetAllocationSize(area));
        allocator->Deallocate(area);
        --NoAllocations;
    }
}
