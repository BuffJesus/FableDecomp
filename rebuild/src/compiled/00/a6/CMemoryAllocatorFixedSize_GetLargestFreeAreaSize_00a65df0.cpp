#include "candidates/fixed_size_allocator_accessors_compiled.h"

unsigned long CMemoryAllocatorFixedSize::GetLargestFreeAreaSize()
{
    CFixedSizeAllocatorNode* current = PFirstChunk;
    if (current != 0)
    {
        unsigned long areaCapacity = NoAreasPerChunk;
        do
        {
            if (areaCapacity - current->NoAllocatedAreas != 0)
                return AreaSize;
            current = current->PNextChunk;
        } while (current != 0);
    }
    return 0;
}
