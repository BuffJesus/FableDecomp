#include "candidates/fixed_size_allocator_accessors_compiled.h"

unsigned long CMemoryAllocatorFixedSize::GetTotalMemoryAllocated()
{
    unsigned long total = 0;
    CFixedSizeAllocatorNode* current = ControlledAreas;
    while (current != 0)
    {
        total += current->AreaCount * AllocationSizeIncludingHeader + HeaderSize;
        current = current->Next;
    }
    return total;
}
