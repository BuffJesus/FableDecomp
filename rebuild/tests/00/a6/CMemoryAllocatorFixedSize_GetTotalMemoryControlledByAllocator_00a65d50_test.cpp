#include <stdio.h>
#include <string.h>
#include "candidates/fixed_size_allocator_accessors_compiled.h"
int main() { fable_u8 s[0xC4C]; memset(s,0,sizeof(s)); CMemoryAllocatorFixedSize* a=reinterpret_cast<CMemoryAllocatorFixedSize*>(s); a->ExtraMemUsedByOS=100; a->ChunkSize=64; if(a->CMemoryAllocatorFixedSize::GetTotalMemoryControlledByAllocator()!=100) return 1; CFixedSizeAllocatorNode n[3]; memset(n,0,sizeof(n)); n[0].PNextChunk=&n[1]; n[1].PNextChunk=&n[2]; a->PFirstChunk=&n[0]; if(a->CMemoryAllocatorFixedSize::GetTotalMemoryControlledByAllocator()!=292) return 2; printf("FIXED_ALLOCATOR_TOTAL_MEMORY_TEST PASS\n"); return 0; }
