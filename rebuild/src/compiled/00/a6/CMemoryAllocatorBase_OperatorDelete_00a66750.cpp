// Retail 0x00A66750; class-specific release bypasses the engine heap.
#include "fable_memory_allocator.h"
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);

void FABLE_FASTCALL CMemoryAllocatorBase::operator delete(void* area)
{
    VirtualFree(area,0,0x8000); // MEM_RELEASE
}
