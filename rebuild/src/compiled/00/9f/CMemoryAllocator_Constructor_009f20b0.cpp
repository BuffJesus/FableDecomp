// Retail 0x009F20B0: preserve separate lookup branches for original code generation.
#include "fable_memory_allocator.h"
#include "candidates/fixed_size_allocator_accessors_compiled.h"
#include "fable_variable_size_allocator.h"
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION*);
void FABLE_FASTCALL InitializeMemorySystem();
static inline bool IsFixedSizeFlag(unsigned long flags)
{
    return (flags>>1)&1;
}

CMemoryAllocator::CMemoryAllocator(void* heap)
    : MemoryAllocated(0),NoAllocations(0),CurrentTrackingID(0)
{
    InitializeCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
    EnterSynchronisation();
    InitializeMemorySystem();
    memset(MaxFixedAllocationSizes,0,sizeof(MaxFixedAllocationSizes));
    memset(AllocSizeToAllocator,0,sizeof(AllocSizeToAllocator));
    memset(AllocatorExists,0,sizeof(AllocatorExists));
    memset(MemoryAllocatedPerTrackingID,0,sizeof(MemoryAllocatedPerTrackingID));
    MemoryAllocated=0;
    NoAllocators=30;
    long alloc_index;
    bool is_fixed_size;
    for(alloc_index=0;alloc_index<NoAllocators;++alloc_index) {
        CAllocatorInfo& info=AllocatorInfo[alloc_index];
        if(info.MaxAllocSize!=0) {
            is_fixed_size=IsFixedSizeFlag(info.InternalFlags);
            unsigned long trackingID=info.Flags;
            AllocatorExists[trackingID]=true;
            if(is_fixed_size) {
                CMemoryAllocatorFixedSize* allocator=new CMemoryAllocatorFixedSize(heap,info.MinChunkSize,
                    info.MaxAllocSize,info.Alignment,trackingID,info.InternalFlags);
                info.PAllocator=allocator;
                info.MaxAllocSize=allocator->GetAllocationSize(0);
                if(info.MaxAllocSize>MaxFixedAllocationSizes[trackingID])
                    MaxFixedAllocationSizes[trackingID]=info.MaxAllocSize;
            } else {
                info.PAllocator=new CMemoryAllocatorVariableSize(heap,info.MinChunkSize,
                    0,info.InitialSize,info.Alignment,trackingID,info.InternalFlags);
            }
        }
    }
    for(long category=0;category<15;++category) {
        long maximum=MaxFixedAllocationSizes[category];
        if(AllocatorExists[category]) {
            __int64 bytes=((__int64)maximum+2)*sizeof(CAllocSizeToAllocator);
            if(bytes<0 || bytes>0x7fffffff)return;
            AllocSizeToAllocator[category]=static_cast<CAllocSizeToAllocator*>(malloc((maximum+2)*sizeof(CAllocSizeToAllocator)));
            for(long size=0;size<=maximum+1;++size) {
                for(alloc_index=0;alloc_index<NoAllocators;++alloc_index) {
                    CAllocatorInfo& info=AllocatorInfo[alloc_index];
                    if(info.Flags==category) {
                        is_fixed_size=IsFixedSizeFlag(info.InternalFlags);
                        if(is_fixed_size) {
                            long minimum=0;
                            if(alloc_index>0)minimum=AllocatorInfo[alloc_index-1].MaxAllocSize+1;
                            if(size<minimum || size>info.MaxAllocSize)continue;
                            AllocSizeToAllocator[category][size].PAllocator=info.PAllocator;
                            AllocSizeToAllocator[category][size].ID=static_cast<unsigned char>(alloc_index);
                            break;
                        } else {
                            AllocSizeToAllocator[category][size].PAllocator=info.PAllocator;
                            AllocSizeToAllocator[category][size].ID=static_cast<unsigned char>(alloc_index);
                            break;
                        }
                    }
                }
            }
        }
    }
    MemoryAuditBuffer=0;
    LeaveSynchronisation();
}
