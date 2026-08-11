#include <cstdio>

static int g_freed = 0;
static void* g_lastFreed = 0;
static int g_baseCalled = 0;

void __cdecl FreeThing(void* p);

struct CTCInventoryItemBase
{
    void* m_vptr;
    void OnDie();
};

struct CTCInventoryItem : public CTCInventoryItemBase
{
    void* m_pField4;
    void OnDie();
};

void __cdecl FreeThing(void* p) { g_freed++; g_lastFreed = p; }
void CTCInventoryItemBase::OnDie() { g_baseCalled++; }

int main()
{
    int dummy = 0x1234;

    // Case 1: field is non-null -> free + base OnDie
    CTCInventoryItem a;
    a.m_vptr = 0;
    a.m_pField4 = &dummy;
    a.OnDie();
    bool c1 = (g_freed == 1) && (g_lastFreed == &dummy) && (g_baseCalled == 1);

    // Case 2: field is null -> no free, still base OnDie
    CTCInventoryItem b;
    b.m_vptr = 0;
    b.m_pField4 = 0;
    b.OnDie();
    bool c2 = (g_freed == 1) && (g_baseCalled == 2);

    if (c1 && c2) { printf("ONDIE_OK\n"); return 0; }
    printf("ONDIE_FAIL freed=%d base=%d\n", g_freed, g_baseCalled);
    return 1;
}