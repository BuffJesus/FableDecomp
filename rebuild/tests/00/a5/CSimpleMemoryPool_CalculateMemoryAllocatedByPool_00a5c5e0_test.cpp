#include <stdio.h>

struct PoolBlock {
    char pad0[0x10];
    unsigned long size;   // +0x10
    PoolBlock* next;      // +0x14
};

struct CSimpleMemoryPool {
    void* pad0;           // +0x00
    PoolBlock* head;      // +0x04

    unsigned long CalculateMemoryAllocatedByPool();
};

unsigned long CSimpleMemoryPool::CalculateMemoryAllocatedByPool()
{
    unsigned long total = 0;
    PoolBlock* b = this->head;
    while (b != 0) {
        total += b->size;
        b = b->next;
    }
    return total;
}

int main()
{
    PoolBlock b0, b1, b2;
    b0.size = 100; b0.next = &b1;
    b1.size = 250; b1.next = &b2;
    b2.size = 33;  b2.next = 0;

    CSimpleMemoryPool p;
    p.pad0 = 0;
    p.head = &b0;

    unsigned long r = p.CalculateMemoryAllocatedByPool();
    if (r != 383) { printf("FAIL got %lu\n", r); return 1; }

    CSimpleMemoryPool e;
    e.pad0 = 0;
    e.head = 0;
    if (e.CalculateMemoryAllocatedByPool() != 0) { printf("FAIL empty\n"); return 1; }

    printf("POOLSUM_OK\n");
    return 0;
}