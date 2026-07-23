#include "candidates/fixed_size_allocator_accessors_compiled.h"

unsigned long CMemoryAllocatorFixedSize::GetNoFreeAreas()
{
    unsigned long total = 0;
    CFixedSizeAllocatorNode* current = ControlledAreas;
    if (current != 0)
    {
        unsigned long areaCapacity = AreaCapacity;
        do
        {
            long areaCount = current->AreaCount;
            current = current->Next;
            total += areaCapacity - areaCount;
        } while (current != 0);
    }
    return total;
}
