// Retail 0x00A66730; class-specific storage is reserved/committed directly.
#include "fable_memory_allocator.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);

void* FABLE_FASTCALL CMemoryAllocatorBase::operator new(unsigned int size)
{
    return VirtualAlloc(0,size,0x3000,4); // MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE
}
