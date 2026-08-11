#include <cstdio>

static int g_freed = 0;
static void* g_freed_ptr = 0;
static int g_base_called = 0;

extern "C" void __cdecl freemem_009d3b70(void* p) { g_freed = 1; g_freed_ptr = p; }

struct CTCInventoryItemBase
{
    void* vtbl;
    void* thing;
    void OnDieBase() { g_base_called = 1; }
};

struct CTCInventoryItem : public CTCInventoryItemBase
{
    void OnDie();
};

void CTCInventoryItem::OnDie()
{
    void* p = this->thing;
    if (p != 0)
        freemem_009d3b70(p);
    this->OnDieBase();
}

int main()
{
    int ok = 1;

    // Case 1: thing non-null -> free called, base called
    CTCInventoryItem a;
    a.vtbl = 0;
    int dummy = 42;
    a.thing = &dummy;
    g_freed = 0; g_freed_ptr = 0; g_base_called = 0;
    a.OnDie();
    if (!(g_freed == 1 && g_freed_ptr == &dummy && g_base_called == 1)) ok = 0;

    // Case 2: thing null -> no free, base still called
    CTCInventoryItem b;
    b.vtbl = 0;
    b.thing = 0;
    g_freed = 0; g_freed_ptr = 0; g_base_called = 0;
    b.OnDie();
    if (!(g_freed == 0 && g_base_called == 1)) ok = 0;

    if (ok) printf("ONDIE_PARITY_OK\n");
    else printf("ONDIE_FAIL\n");
    return ok ? 0 : 1;
}