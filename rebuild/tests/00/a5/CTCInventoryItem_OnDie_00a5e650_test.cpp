#include <stdio.h>

void __cdecl FreeThing(void* p);

struct CTCInventoryItem
{
    void* m_field0;
    void* m_thing;
    void OnDie();
    void BaseOnDie();
};

static int g_freed = 0;
static void* g_freedPtr = 0;
static int g_baseCalled = 0;

void __cdecl FreeThing(void* p) { g_freed++; g_freedPtr = p; }
void CTCInventoryItem::BaseOnDie() { g_baseCalled++; }

int main()
{
    // Case 1: m_thing non-null -> free + base
    int dummy = 123;
    CTCInventoryItem a;
    a.m_field0 = 0;
    a.m_thing = &dummy;
    a.OnDie();
    if (g_freed != 1 || g_freedPtr != &dummy || g_baseCalled != 1) {
        printf("FAIL case1\n"); return 1;
    }

    // Case 2: m_thing null -> no free, base still called
    CTCInventoryItem b;
    b.m_field0 = 0;
    b.m_thing = 0;
    a.m_thing = 0; // avoid confusion
    b.OnDie();
    if (g_freed != 1 || g_baseCalled != 2) {
        printf("FAIL case2\n"); return 1;
    }

    printf("ONDIE_OK\n");
    return 0;
}