#include <stdio.h>

struct Entry {
    char pad[0xf];
    unsigned char locked;
};

struct CHandleBasedMemoryPool {
    char pad0[0x18];
    int lockedCount;
    char pad1[0x438 - 0x1c];
    Entry** table;
};

void __fastcall Unlock(CHandleBasedMemoryPool* self, int edxDummy, long index)
{
    Entry* e = self->table[index];
    if (e->locked) {
        e->locked = 0;
        self->lockedCount--;
    }
}

int main()
{
    CHandleBasedMemoryPool pool;
    Entry a, b;
    Entry* tbl[2];
    tbl[0] = &a; tbl[1] = &b;
    pool.table = tbl;
    pool.lockedCount = 5;
    a.locked = 1;
    b.locked = 0;

    // Unlock index 0: was locked -> clears + decrements
    Unlock(&pool, 0, 0);
    // Unlock index 1: not locked -> no change
    Unlock(&pool, 0, 1);
    // Unlock index 0 again: already cleared -> no change
    Unlock(&pool, 0, 0);

    int ok = (a.locked == 0) && (b.locked == 0) && (pool.lockedCount == 4);
    if (ok) printf("UNLOCK_OK\n");
    else printf("UNLOCK_FAIL lc=%d a=%d b=%d\n", pool.lockedCount, a.locked, b.locked);
    return 0;
}