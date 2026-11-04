#pragma once
#include "fable_memory_allocator.h"

// FableWin.exe's debug body (02CB6E30) tests the allocation after entering the
// lock. Callers whose retail code tests it first have their own outer `if`,
// which the optimiser threads through the local.
__forceinline void FABLE_FASTCALL CMemoryAllocator::Deallocate(void* allocation)
{
    EnterSynchronisation();
    if (allocation && Get()) {
        Get()->InternalDeallocate(allocation);
        if (g_FableAllocatorShutdownRequested_013CAA94 && Get()->GetNoAllocations()==0) {
            delete Get();
            g_FableMemoryAllocator_013CAA90=0;
        }
    }
    LeaveSynchronisation();
}
