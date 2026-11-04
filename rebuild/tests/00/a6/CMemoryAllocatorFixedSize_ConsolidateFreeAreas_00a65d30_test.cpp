#include <stdio.h>
#include "candidates/fixed_size_allocator_accessors_compiled.h"
int main() { fable_u8 s[0xC4C]; CMemoryAllocatorFixedSize* a=reinterpret_cast<CMemoryAllocatorFixedSize*>(s); a->CMemoryAllocatorFixedSize::ConsolidateFreeAreas(); printf("FIXED_ALLOCATOR_CONSOLIDATE_TEST PASS\n"); return 0; }
