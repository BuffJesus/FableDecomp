#include "candidates/fixed_size_allocator_accessors_compiled.h"

unsigned long CMemoryAllocatorFixedSize::GetTotalMemoryControlledByAllocator()
{
    CFixedSizeAllocatorNode* current = ControlledAreas;
    unsigned long total = BaseControlledMemory;
    if (current != 0)
    {
        do
        {
            current = current->Next;
            total += ControlledAreaSize;
        } while (current != 0);
    }
    return total;
}
