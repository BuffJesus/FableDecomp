#include "candidates/fixed_size_allocator_accessors_compiled.h"

struct FixedAllocatorTotalNodeOverlay
{
    fable_u8 Unknown00[4];
    unsigned long AreaCount;
    FixedAllocatorTotalNodeOverlay* Next;
};

struct FixedAllocatorTotalOverlay
{
    fable_u8 Unknown0000[0xC1C];
    FixedAllocatorTotalNodeOverlay* ControlledAreas;
    fable_u8 UnknownC20[0x0C];
    unsigned long AllocationSizeIncludingHeader;
    unsigned long HeaderSize;
};

FABLE_STATIC_ASSERT(offsetof(FixedAllocatorTotalNodeOverlay, AreaCount) == 0x04);
FABLE_STATIC_ASSERT(offsetof(FixedAllocatorTotalNodeOverlay, Next) == 0x08);
FABLE_STATIC_ASSERT(offsetof(FixedAllocatorTotalOverlay, ControlledAreas) == 0xC1C);
FABLE_STATIC_ASSERT(offsetof(FixedAllocatorTotalOverlay, AllocationSizeIncludingHeader) == 0xC2C);
FABLE_STATIC_ASSERT(offsetof(FixedAllocatorTotalOverlay, HeaderSize) == 0xC30);

unsigned long CMemoryAllocatorFixedSize::GetTotalMemoryAllocated()
{
    unsigned long total = 0;
    FixedAllocatorTotalOverlay* self = reinterpret_cast<FixedAllocatorTotalOverlay*>(this);
    FixedAllocatorTotalNodeOverlay* current = self->ControlledAreas;
    if (current != 0)
    {
        do
        {
            unsigned long areaCount = current->AreaCount;
            current = current->Next;
            total = total + areaCount * self->AllocationSizeIncludingHeader + self->HeaderSize;
        } while (current != 0);
    }
    return total;
}
