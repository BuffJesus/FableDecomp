#ifndef FABLETLC_FIXED_SIZE_ALLOCATOR_ACCESSORS_COMPILED_H
#define FABLETLC_FIXED_SIZE_ALLOCATOR_ACCESSORS_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

struct CFixedSizeAllocatorNode
{
    fable_u8 Unknown00[4];
    long AreaCount;
    CFixedSizeAllocatorNode* Next;
};

class CMemoryAllocatorFixedSize
{
public:
    fable_u8 Unknown0000[0xC1C];
    CFixedSizeAllocatorNode* ControlledAreas;
    fable_u8 UnknownC20[8];
    unsigned long AllocationSize;
    unsigned long AllocationSizeIncludingHeader;
    unsigned long HeaderSize;
    unsigned long AreaCapacity;
    unsigned long ControlledAreaSize;
    fable_u8 UnknownC3C[8];
    unsigned long BaseControlledMemory;

    void ConsolidateFreeAreas();
    unsigned long GetAllocationSize(void* allocation);
    unsigned long GetTotalMemoryControlledByAllocator();
    unsigned long GetAllocationSizeIncludingHeader(void* allocation);
    unsigned long GetHeaderOverhead();
    unsigned long GetLargestFreeAreaSize();
    unsigned long GetNoFreeAreas();
    unsigned long GetNoAllocatedAreas();
    unsigned long GetTotalMemoryAllocated();
    float GetFragmentation();
};

FABLE_STATIC_ASSERT(offsetof(CFixedSizeAllocatorNode, AreaCount) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CFixedSizeAllocatorNode, Next) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CMemoryAllocatorFixedSize, ControlledAreas) == 0xC1C);
FABLE_STATIC_ASSERT(offsetof(CMemoryAllocatorFixedSize, AllocationSize) == 0xC28);
FABLE_STATIC_ASSERT(offsetof(CMemoryAllocatorFixedSize, AllocationSizeIncludingHeader) == 0xC2C);
FABLE_STATIC_ASSERT(offsetof(CMemoryAllocatorFixedSize, HeaderSize) == 0xC30);
FABLE_STATIC_ASSERT(offsetof(CMemoryAllocatorFixedSize, AreaCapacity) == 0xC34);
FABLE_STATIC_ASSERT(offsetof(CMemoryAllocatorFixedSize, ControlledAreaSize) == 0xC38);
FABLE_STATIC_ASSERT(offsetof(CMemoryAllocatorFixedSize, BaseControlledMemory) == 0xC44);

#endif
