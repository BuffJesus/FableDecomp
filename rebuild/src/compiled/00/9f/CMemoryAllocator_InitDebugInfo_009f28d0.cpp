#include "fable_memory_allocator.h"
void CMemoryAllocator::InitDebugInfo(void* allocation,bool debug,long requestedSize)
{
    GetAllocationSize(allocation);
    unsigned long* metadata=reinterpret_cast<unsigned long*>(static_cast<unsigned char*>(allocation)+GetAllocationSize(allocation)-20);
    metadata[0]=0xffffffff;
    metadata[1]=CurrentTrackingID;
    metadata[2]=requestedSize;
    *(reinterpret_cast<unsigned long*>(allocation)-1)=0xffffffff;
    MemoryAllocatedPerTrackingID[CurrentTrackingID]+=requestedSize;
}
