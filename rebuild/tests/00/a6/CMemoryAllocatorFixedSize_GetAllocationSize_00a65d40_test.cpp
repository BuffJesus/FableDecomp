#include <stdio.h>
#include "candidates/fixed_size_allocator_accessors_compiled.h"
int main() { fable_u8 s[0xC48]; CMemoryAllocatorFixedSize* a=reinterpret_cast<CMemoryAllocatorFixedSize*>(s); a->AllocationSize=96; if(a->GetAllocationSize(a)!=96) return 1; if(a->GetAllocationSize(0)!=96) return 2; printf("FIXED_ALLOCATOR_GET_SIZE_TEST PASS\n"); return 0; }
