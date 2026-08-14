#include <cstdio>
typedef unsigned long DWORD;

struct CTCInventoryItem {
    DWORD vtbl;
    char pad[0x30];
    void* p34;
};

static int g_freed = 0;
static void* g_freedptr = 0;
static int g_basecalled = 0;

void FreeHelper(void* p){ g_freed++; g_freedptr = p; }
void BaseOnDie(void* self){ g_basecalled++; }

void OnDie(CTCInventoryItem* self)
{
    if (self->p34 != 0)
        FreeHelper(self->p34);
    self->vtbl = (DWORD)0x1230ba0;
    BaseOnDie(self);
}

int main()
{
    // Case 1: p34 non-null -> free called, vtbl set, base called
    CTCInventoryItem a;
    a.vtbl = 0; a.p34 = (void*)0xDEAD;
    OnDie(&a);
    if (g_freed != 1) { printf("FAIL free\n"); return 1; }
    if (g_freedptr != (void*)0xDEAD) { printf("FAIL freeptr\n"); return 1; }
    if (a.vtbl != (DWORD)0x1230ba0) { printf("FAIL vtbl\n"); return 1; }
    if (g_basecalled != 1) { printf("FAIL base\n"); return 1; }

    // Case 2: p34 null -> no free, still vtbl+base
    CTCInventoryItem b;
    b.vtbl = 0x99; b.p34 = 0;
    OnDie(&b);
    if (g_freed != 1) { printf("FAIL free2\n"); return 1; }
    if (b.vtbl != (DWORD)0x1230ba0) { printf("FAIL vtbl2\n"); return 1; }
    if (g_basecalled != 2) { printf("FAIL base2\n"); return 1; }

    printf("BEHAVIOR_OK\n");
    return 0;
}