#include "candidates/fixed_size_allocator_accessors_compiled.h"

unsigned long CMemoryAllocatorFixedSize::GetNoFreeAreas()
{
    unsigned long total = 0;
    CFixedSizeAllocatorNode* current = ControlledAreas;
    while (current != 0)
    {
        total += AreaCapacity - current->AreaCount;
        current = current->Next;
    }
    return total;
}
