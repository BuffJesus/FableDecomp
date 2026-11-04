#include "candidates/fixed_size_allocator_accessors_compiled.h"

unsigned long CMemoryAllocatorFixedSize::GetNoFreeAreas()
{
    unsigned long total = 0;
    CFixedSizeAllocatorNode* current = PFirstChunk;
    while (current != 0)
    {
        total += NoAreasPerChunk - current->NoAllocatedAreas;
        current = current->PNextChunk;
    }
    return total;
}
