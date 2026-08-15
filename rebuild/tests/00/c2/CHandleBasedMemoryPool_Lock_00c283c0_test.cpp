#include <stdio.h>

struct Entry {
    char pad[0xf];
    unsigned char flag;
};

struct CHandleBasedMemoryPool {
    char pad0[0x18];
    unsigned long lockCount;
    char pad1[0x440 - 0x18 - 4];
    Entry** table;
};

void __fastcall Lock(CHandleBasedMemoryPool* self, int, long index)
{
    Entry** base = self->table;
    Entry* volatile* slot = base + index;
    Entry* e = *slot;
    if (e->flag == 0) {
        e->flag = 1;
        self->lockCount++;
    }
}

int main()
{
    CHandleBasedMemoryPool pool;
    for (int i = 0; i < (int)sizeof(pool); ++i) ((char*)&pool)[i] = 0;

    Entry e0, e1;
    e0.flag = 0;
    e1.flag = 0;
    Entry* tbl[4];
    tbl[0] = &e0;
    tbl[1] = &e1;
    tbl[2] = &e0;
    tbl[3] = &e1;
    pool.table = tbl;
    pool.lockCount = 0;

    Lock(&pool, 0, 0);
    if (!(e0.flag == 1 && pool.lockCount == 1)) { printf("BAD1\n"); return 1; }

    Lock(&pool, 0, 0);
    if (!(e0.flag == 1 && pool.lockCount == 1)) { printf("BAD2\n"); return 1; }

    Lock(&pool, 0, 1);
    if (!(e1.flag == 1 && pool.lockCount == 2)) { printf("BAD3\n"); return 1; }

    printf("LOCK_OK\n");
    return 0;
}