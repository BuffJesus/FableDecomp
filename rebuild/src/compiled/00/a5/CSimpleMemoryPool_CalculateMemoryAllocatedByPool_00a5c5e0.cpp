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