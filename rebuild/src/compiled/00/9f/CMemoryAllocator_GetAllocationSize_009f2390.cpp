#include "fable_memory_allocator.h"
unsigned long CMemoryAllocator::GetAllocationSize(void* allocation)
{
    EnterSynchronisation();
    if(!allocation)return 0;
    unsigned char id;
    unsigned long address=reinterpret_cast<unsigned long>(allocation);
    if(address>=g_FableAllocatorRangeStart_013CAAA4&&address<g_FableAllocatorRangeEnd_013CAAA8)
        id=static_cast<unsigned char>((address-g_FableAllocatorRangeStart_013CAAA4)>>24);
    else id=*(static_cast<unsigned char*>(allocation)-5);
    if(id>=NoAllocators)return 0;
    unsigned long size=AllocatorInfo[id].PAllocator->GetAllocationSize(static_cast<unsigned char*>(allocation)-4);
    LeaveSynchronisation();
    return size;
}
