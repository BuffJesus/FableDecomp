// Retail static aligned API: exact79/108 bytes, independently replayed and actual Win32 composition verified.
#include "fable_memory_allocator.h"
void* FABLE_FASTCALL CMemoryAllocator::AllocateAligned(unsigned long size,long alignment)
{
    if(!Get())Create();
    EnterSynchronisation();
    void* allocation=Get()->InternalAllocate(size+alignment+3,0,false);
    LeaveSynchronisation();
    unsigned long address=(reinterpret_cast<unsigned long>(allocation)+alignment+3)&~(alignment-1);
    void* aligned=reinterpret_cast<void*>(address);
    *(reinterpret_cast<void**>(aligned)-1)=allocation;
    return aligned;
}
void FABLE_FASTCALL CMemoryAllocator::DeallocateAligned(void* allocation)
{
    if(allocation){
        void* original=*(reinterpret_cast<void**>(allocation)-1);
        EnterSynchronisation();
        if(original&&Get()){
            Get()->InternalDeallocate(original);
            if(g_FableAllocatorShutdownRequested_013CAA94&&Get()->NoAllocations==0){
                delete Get();
                g_FableMemoryAllocator_013CAA90=0;
            }
        }
        LeaveSynchronisation();
    }
}
