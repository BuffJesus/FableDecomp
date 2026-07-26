#include <math.h>
#include <stdio.h>
#include "candidates/fixed_size_allocator_accessors_compiled.h"

extern "C" __declspec(dllimport) void* FABLE_STDCALL VirtualAlloc(
    void*, unsigned long, unsigned long, unsigned long);
extern "C" __declspec(dllimport) int FABLE_STDCALL VirtualFree(
    void*, unsigned long, unsigned long);

static const unsigned long kMemCommit = 0x1000;
static const unsigned long kMemReserve = 0x2000;
static const unsigned long kMemRelease = 0x8000;
static const unsigned long kPageReadWrite = 0x04;

int main()
{
    void* page = VirtualAlloc(reinterpret_cast<void*>(0x0122D000), 0x1000,
        kMemReserve | kMemCommit, kPageReadWrite);
    if (page == 0) return 1;
    *reinterpret_cast<float*>(0x0122DEDC) = 0.375f;
    CMemoryAllocatorFixedSize* allocator = 0;
    if (fabs(allocator->GetFragmentation() - 0.375f) > 0.0001f) return 2;
    VirtualFree(page, 0, kMemRelease);
    printf("FIXED_ALLOCATOR_FRAGMENTATION_TEST PASS\n");
    return 0;
}
