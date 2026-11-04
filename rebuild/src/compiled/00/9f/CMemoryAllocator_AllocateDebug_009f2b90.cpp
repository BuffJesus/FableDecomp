// Retail debug allocation. Compile with /GS: native stack-cookie behavior is required.
#include "fable_memory_allocator.h"
void* FABLE_FASTCALL CMemoryAllocator::Allocate(unsigned long size,unsigned long category,const char* file,unsigned long line,const char* function)
{
    if(!Get())Create();
    EnterSynchronisation();
    void* allocation=Get()->InternalAllocate(size,category,file,line,function);
    LeaveSynchronisation();
    return allocation;
}
