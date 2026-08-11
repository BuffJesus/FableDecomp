#include <stdio.h>

static int g_freed = 0;
static void* g_freed_ptr = 0;
static int g_base_called = 0;

extern "C" void __cdecl FreeHelper(void* p) { g_freed++; g_freed_ptr = p; }

struct CInventoryItemBase
{
    void OnDie();
};
void CInventoryItemBase::OnDie() { g_base_called++; }

struct CTCInventoryItem : public CInventoryItemBase
{
    char pad[0x10];
    void* m_ptr;
    void OnDie();
};

void CTCInventoryItem::OnDie()
{
    void* p = this->m_ptr;
    if (p != 0)
        FreeHelper(p);
    CInventoryItemBase::OnDie();
}

int main()
{
    CTCInventoryItem a;
    int dummy = 0x1234;
    a.m_ptr = &dummy;
    a.OnDie();
    // with ptr -> freed once, base called
    int ok1 = (g_freed == 1) && (g_freed_ptr == &dummy) && (g_base_called == 1);

    CTCInventoryItem b;
    b.m_ptr = 0;
    g_freed = 0; g_base_called = 0;
    b.OnDie();
    // null ptr -> not freed, base still called
    int ok2 = (g_freed == 0) && (g_base_called == 1);

    if (ok1 && ok2)
        printf("ONDIE_PARITY_OK\n");
    else
        printf("FAIL f=%d b=%d\n", g_freed, g_base_called);
    return (ok1 && ok2) ? 0 : 1;
}