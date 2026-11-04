// Retail shutdown helper: defer release while engine allocations remain alive.
// No native callers or address-taken references have been established yet.
#include "fable_memory_allocator.h"
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);

void FABLE_CDECL FableMemoryAllocatorShutdown_009f2d20()
{
    if(g_FableMemoryAllocator_013CAA90) {
        g_FableAllocatorShutdownRequested_013CAA94=true;
        if(g_FableMemoryAllocator_013CAA90->NoAllocations==0) {
            delete g_FableMemoryAllocator_013CAA90;
            g_FableMemoryAllocator_013CAA90=0;
        }
    }
}
