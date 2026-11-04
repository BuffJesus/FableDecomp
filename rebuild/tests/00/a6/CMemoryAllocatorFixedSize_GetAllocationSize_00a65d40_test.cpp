#include <stdio.h>
#include "candidates/fixed_size_allocator_accessors_compiled.h"
int main() { fable_u8 s[0xC4C]; CMemoryAllocatorFixedSize* a=reinterpret_cast<CMemoryAllocatorFixedSize*>(s); a->AreaSize=96; if(a->CMemoryAllocatorFixedSize::GetAllocationSize(a)!=96) return 1; if(a->CMemoryAllocatorFixedSize::GetAllocationSize(0)!=96) return 2; printf("FIXED_ALLOCATOR_GET_SIZE_TEST PASS\n"); return 0; }
