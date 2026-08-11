#include <cstdio>

static int g_freed = 0;
static void* g_freed_ptr = 0;
static int g_base_called = 0;

extern "C" void __cdecl free_thing(void* p) { g_freed++; g_freed_ptr = p; }

struct CTCInventoryItemBase {
    char pad[0x40];
    void* m_ptr;   // +0x40
    void BaseOnDie();
};
void CTCInventoryItemBase::BaseOnDie() { g_base_called++; }

struct CTCInventoryItem : public CTCInventoryItemBase {
    void OnDie();
};
void CTCInventoryItem::OnDie()
{
    if (this->m_ptr)
        free_thing(this->m_ptr);
    this->BaseOnDie();
}

int main()
{
    int ok = 1;

    // Case 1: non-null ptr -> freed + base called
    CTCInventoryItem a;
    int dummy = 5;
    a.m_ptr = &dummy;
    g_freed = 0; g_base_called = 0; g_freed_ptr = 0;
    a.OnDie();
    if (g_freed != 1) ok = 0;
    if (g_freed_ptr != &dummy) ok = 0;
    if (g_base_called != 1) ok = 0;

    // Case 2: null ptr -> not freed, base still called
    CTCInventoryItem b;
    b.m_ptr = 0;
    g_freed = 0; g_base_called = 0;
    b.OnDie();
    if (g_freed != 0) ok = 0;
    if (g_base_called != 1) ok = 0;

    if (ok) printf("ONDIE_OK\n");
    else    printf("ONDIE_FAIL\n");
    return ok ? 0 : 1;
}