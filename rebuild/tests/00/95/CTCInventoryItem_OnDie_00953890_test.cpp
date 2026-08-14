#include <stdio.h>

struct CTCInventoryItem {
    char pad[0x120];
    void* m120;
};

static int g_freed = 0;
static int g_baseCalled = 0;
static void* g_freedPtr = 0;

void __cdecl freeHelper(void* p) { g_freed++; g_freedPtr = p; }
void __fastcall Base_OnDie(void* self) { g_baseCalled++; }

void __fastcall OnDie(CTCInventoryItem* self)
{
    void* p = self->m120;
    if (p != 0)
        freeHelper(p);
    Base_OnDie(self);
}

int main()
{
    // Case 1: member non-null -> free + base
    CTCInventoryItem a;
    int dummy = 42;
    a.m120 = &dummy;
    OnDie(&a);
    if (g_freed != 1 || g_freedPtr != &dummy || g_baseCalled != 1) { printf("FAIL1\n"); return 1; }

    // Case 2: member null -> no free, still base
    CTCInventoryItem b;
    b.m120 = 0;
    OnDie(&b);
    if (g_freed != 1 || g_baseCalled != 2) { printf("FAIL2\n"); return 1; }

    printf("ONDIE_OK\n");
    return 0;
}