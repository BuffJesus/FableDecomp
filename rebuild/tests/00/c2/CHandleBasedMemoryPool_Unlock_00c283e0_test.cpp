#include <stdio.h>
typedef unsigned char uchar;

struct PoolEntry {
    char pad[0xf];
    uchar locked;   // +0xf
};

struct CHandleBasedMemoryPool {
    char pad0[0x18];
    int lockCount;          // +0x18
    char pad1[0x440 - 0x1c];
    PoolEntry** entries;    // +0x440

    void Unlock(long handle);
};

void CHandleBasedMemoryPool::Unlock(long handle)
{
    PoolEntry** slot = &this->entries[handle];
    if ((*slot)->locked != 0) {
        (*slot)->locked = 0;
        this->lockCount--;
    }
}

int main()
{
    CHandleBasedMemoryPool pool;
    pool.lockCount = 5;
    PoolEntry e0, e1, e2;
    e0.locked = 1; e1.locked = 0; e2.locked = 7;
    PoolEntry* arr[3] = { &e0, &e1, &e2 };
    pool.entries = arr;

    pool.Unlock(0);
    if (e0.locked != 0) { printf("FAIL0\n"); return 1; }
    if (pool.lockCount != 4) { printf("FAIL_CNT0\n"); return 1; }

    pool.Unlock(1);
    if (e1.locked != 0) { printf("FAIL1\n"); return 1; }
    if (pool.lockCount != 4) { printf("FAIL_CNT1\n"); return 1; }

    pool.Unlock(2);
    if (e2.locked != 0) { printf("FAIL2\n"); return 1; }
    if (pool.lockCount != 3) { printf("FAIL_CNT2\n"); return 1; }

    printf("UNLOCK_OK\n");
    return 0;
}