// Native cleanup provider: retail parity and behavior evidence enforced by permanent gates.
#define FABLE_MEMORY_ALLOCATOR_OUT_OF_LINE_SUPPORT
#include "fable_memory_allocator.h"
#include "fable_file_installer.h"
// Shared inline support is emitted as COMDAT and coalesces with other native
// units. Keep retail's out-of-line boundaries in the range destructor.
inline __declspec(noinline) void FABLE_FASTCALL CMemoryAllocator::operator delete(void* area) { VirtualFree(area,0,0x8000); }
inline __declspec(noinline) void FABLE_FASTCALL CMemoryAllocator::EnterSynchronisation() { EnterCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70); }
inline __declspec(noinline) void FABLE_FASTCALL CMemoryAllocator::LeaveSynchronisation() { LeaveCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70); }
void FABLE_FASTCALL FableInstallerDestroyBufferRange(InstallerBufferChunk* first,InstallerBufferChunk* last,void*)
{
    while(first!=last) {
        void* allocation=first->allocation;
        if(allocation) {
            CMemoryAllocator::EnterSynchronisation();
            if(allocation && CMemoryAllocator::Get()) {
                CMemoryAllocator::Get()->InternalDeallocate(allocation);
                if(g_FableAllocatorShutdownRequested_013CAA94 && CMemoryAllocator::Get()->GetNoAllocations()==0) {
                    delete CMemoryAllocator::Get();
                    g_FableMemoryAllocator_013CAA90=0;
                }
            }
            CMemoryAllocator::LeaveSynchronisation();
        }
        ++first;
    }
}
