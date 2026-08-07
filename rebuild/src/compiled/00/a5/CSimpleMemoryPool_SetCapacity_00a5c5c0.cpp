#include "rebuild_abi.h"
// CSimpleMemoryPool::SetCapacity @ 0x00a5c5c0
// mov eax,[esp+4]; add eax,0xf; shr eax,4; shl eax,4; mov [ecx+8],eax; ret 4
// Round the requested size up to the next multiple of 16 and store as capacity.
struct CSimpleMemoryPool {
    char pad00[8];
    unsigned int capacity; // +0x8
    void SetCapacity(unsigned int n);
};
void CSimpleMemoryPool::SetCapacity(unsigned int n)
{
    capacity = ((n + 0xf) / 16) * 16;
}