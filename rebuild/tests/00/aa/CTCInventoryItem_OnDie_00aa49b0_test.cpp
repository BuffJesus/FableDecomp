#include <cstdio>

static int g_freed = 0;
static void* g_freed_ptr = 0;
static int g_base_called = 0;

void __cdecl free_target(void* p) { g_freed++; g_freed_ptr = p; }

struct CTCInventoryItem
{
    char pad0[0x10];
    void* m_ptr;          // +0x10
    void BaseOnDie();
    void OnDie_impl();
};

void CTCInventoryItem::BaseOnDie() { g_base_called++; }

void CTCInventoryItem::OnDie_impl()
{
    void* p = this->m_ptr;
    if (p)
        free_target(p);
    this->BaseOnDie();
}

int main()
{
    int dummy = 0;
    CTCInventoryItem a;
    a.m_ptr = &dummy;
    a.OnDie_impl();
    // freed once, base called once
    bool ok1 = (g_freed == 1) && (g_freed_ptr == &dummy) && (g_base_called == 1);

    CTCInventoryItem b;
    b.m_ptr = 0;
    a.OnDie_impl();      // reset check below via counts
    g_freed = 0; g_base_called = 0; g_freed_ptr = 0;
    b.OnDie_impl();
    bool ok2 = (g_freed == 0) && (g_base_called == 1);

    if (ok1 && ok2) { printf("ONDIE_OK\n"); return 0; }
    printf("ONDIE_FAIL\n"); return 1;
}