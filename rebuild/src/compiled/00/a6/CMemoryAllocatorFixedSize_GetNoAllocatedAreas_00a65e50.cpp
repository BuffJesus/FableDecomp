#include "candidates/fixed_size_allocator_accessors_compiled.h"

unsigned long CMemoryAllocatorFixedSize::GetNoAllocatedAreas()
{
    unsigned long total = 0;
    for (CFixedSizeAllocatorNode* current = ControlledAreas; current != 0; current = current->Next)
        total += current->AreaCount;
    return total;
}
