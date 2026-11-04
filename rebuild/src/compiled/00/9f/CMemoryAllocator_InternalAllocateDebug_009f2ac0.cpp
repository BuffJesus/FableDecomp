// Retail debug allocation. Compile with /GS: native stack-cookie behavior is required.
#include "fable_memory_allocator.h"
void* CMemoryAllocator::InternalAllocate(long size,unsigned long category,const char*,long,const char*)
{
    char record[12];
    void* allocation=InternalAllocate(size,category,true);
    GetAllocationSize(allocation);
    unsigned long* metadata=reinterpret_cast<unsigned long*>(static_cast<unsigned char*>(allocation)+GetAllocationSize(allocation)-20);
    reinterpret_cast<unsigned long*>(record)[0]=0xffffffff;
    memcpy(metadata,record,4);
    reinterpret_cast<unsigned long*>(record)[1]=CurrentTrackingID;
    memcpy(metadata+1,record+4,4);
    reinterpret_cast<long*>(record)[2]=size;
    memcpy(metadata+2,record+8,4);
    return allocation;
}
