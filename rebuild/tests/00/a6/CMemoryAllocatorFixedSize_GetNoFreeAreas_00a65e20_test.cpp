#include <stdio.h>
#include <string.h>
#include "candidates/fixed_size_allocator_accessors_compiled.h"

int main()
{
    fable_u8 storage[0xC48];
    CFixedSizeAllocatorNode nodes[2];
    memset(storage, 0, sizeof(storage));
    memset(nodes, 0, sizeof(nodes));
    CMemoryAllocatorFixedSize* allocator = reinterpret_cast<CMemoryAllocatorFixedSize*>(storage);
    allocator->AreaCapacity = 10;
    nodes[0].AreaCount = 7;
    nodes[0].Next = &nodes[1];
    nodes[1].AreaCount = 4;
    allocator->ControlledAreas = &nodes[0];
    if (allocator->GetNoFreeAreas() != 9) return 1;
    printf("FIXED_ALLOCATOR_NO_FREE_AREAS_TEST PASS\n");
    return 0;
}
