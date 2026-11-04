#include <stdio.h>

struct Entry { unsigned char locked; };

struct Pool {
    unsigned long lockCount;
    Entry** entries;
};

void doLock(Pool* self, long handle)
{
    Entry* e = self->entries[handle];
    if (e->locked == 0) {
        e->locked = 1;
        self->lockCount++;
    }
}

int main()
{
    Entry e0; e0.locked = 0;
    Entry e1; e1.locked = 0;
    Entry e2; e2.locked = 1;   // already locked
    Entry* arr[3] = { &e0, &e1, &e2 };

    Pool p; p.lockCount = 0; p.entries = arr;

    doLock(&p, 0);   // locks e0, count -> 1
    doLock(&p, 0);   // no-op (already locked)
    doLock(&p, 1);   // locks e1, count -> 2
    doLock(&p, 2);   // no-op (already locked)

    int ok = (e0.locked == 1) && (e1.locked == 1) && (e2.locked == 1)
             && (p.lockCount == 2);

    if (ok) printf("LOCK_OK\n");
    else printf("LOCK_FAIL count=%lu\n", p.lockCount);
    return 0;
}