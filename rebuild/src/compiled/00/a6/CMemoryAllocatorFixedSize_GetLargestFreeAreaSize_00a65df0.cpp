#include "candidates/fixed_size_allocator_accessors_compiled.h"

unsigned long CMemoryAllocatorFixedSize::GetLargestFreeAreaSize()
{
    CFixedSizeAllocatorNode* current = ControlledAreas;
    if (current != 0)
    {
        unsigned long areaCapacity = AreaCapacity;
        do
        {
            if (areaCapacity - current->AreaCount != 0)
                return AllocationSize;
            current = current->Next;
        } while (current != 0);
    }
    return 0;
}
