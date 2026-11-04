// Retail 0x009F2870; allocate storage before asking for the process heap.
#include "fable_memory_allocator.h"
#include <new>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) void* __stdcall GetProcessHeap();
struct FableMemoryStatus {
    unsigned long Length,Load,TotalPhysical,AvailablePhysical;
    unsigned long TotalPageFile,AvailablePageFile,TotalVirtual,AvailableVirtual;
};
extern "C" __declspec(dllimport) void __stdcall GlobalMemoryStatus(FableMemoryStatus*);

void FABLE_FASTCALL CMemoryAllocator::Create()
{
    void* storage=VirtualAlloc(0,sizeof(CMemoryAllocator),0x3000,4);
    g_FableMemoryAllocator_013CAA90=storage ? ::new(storage) CMemoryAllocator(GetProcessHeap()) : 0;
    FableMemoryStatus status;
    GlobalMemoryStatus(&status);
    MemoryUseBeforeAllocator=status.TotalPhysical-status.AvailablePhysical;
    g_FableAllocatorMemoryBaseline_013CAAB0=MemoryUseBeforeAllocator;
}
