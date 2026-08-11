#include <cstdio>

static int g_freed = 0;
static void* g_freedPtr = 0;
static int g_baseCalled = 0;

extern "C" void __cdecl FreeHelper_bfea14(void* p)
{
    g_freed++;
    g_freedPtr = p;
}

struct CTCInventoryItemBase
{
    void BaseOnDie();
};

struct CTCInventoryItem : public CTCInventoryItemBase
{
    char pad0[0x10];
    void* member10;
    void OnDie();
};

void CTCInventoryItemBase::BaseOnDie()
{
    g_baseCalled++;
}

void CTCInventoryItem::OnDie()
{
    if (this->member10)
        FreeHelper_bfea14(this->member10);
    this->BaseOnDie();
}

int main()
{
    // Case 1: non-null member -> freed once, base called
    CTCInventoryItem a;
    int dummy = 0x1234;
    a.member10 = &dummy;
    a.OnDie();
    bool ok1 = (g_freed == 1) && (g_freedPtr == &dummy) && (g_baseCalled == 1);

    // Case 2: null member -> not freed, base still called
    CTCInventoryItem b;
    b.member10 = 0;
    b.OnDie();
    bool ok2 = (g_freed == 1) && (g_baseCalled == 2);

    if (ok1 && ok2)
        printf("OnDie_PARITY_OK\n");
    else
        printf("FAIL f=%d bc=%d\n", g_freed, g_baseCalled);
    return (ok1 && ok2) ? 0 : 1;
}