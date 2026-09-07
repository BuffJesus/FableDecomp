#include "engine/CSimpleMemoryPool.h"  // retyped onto the PDB layout; byte parity re-verified
struct CMemoryBlock {
    char pad0[0x10];
    unsigned long size;   // +0x10
    CMemoryBlock* next;      // +0x14
};

struct CSimpleMemoryPool_Methods : CSimpleMemoryPool {
    // +0x04

    unsigned long CalculateMemoryAllocatedByPool();
};

unsigned long CSimpleMemoryPool_Methods::CalculateMemoryAllocatedByPool()
{
    unsigned long total = 0;
    CMemoryBlock* b = this->FirstMemoryBlock;
    while (b != 0) {
        total += b->size;
        b = b->next;
    }
    return total;
}