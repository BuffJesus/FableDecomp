#include <cstdio>

struct CTCInventoryItem {
    void* vtbl;
    char  pad[0x24];
    void* m28;
};

static int g_freed = 0;
static void* g_freed_ptr = 0;
static int g_base_called = 0;

void my_delete(void* p) { g_freed++; g_freed_ptr = p; }
void base_OnDie(CTCInventoryItem* self) { g_base_called++; }

void OnDie(CTCInventoryItem* self)
{
    if (self->m28)
        my_delete(self->m28);
    self->vtbl = (void*)0x1230ba0;
    base_OnDie(self);
}

int main()
{
    int dummy = 0;
    CTCInventoryItem a;
    a.vtbl = 0; a.m28 = &dummy;
    OnDie(&a);
    // m28 non-null => delete called once, vtbl set, base called
    bool ok1 = (g_freed == 1) && (g_freed_ptr == &dummy)
        && (a.vtbl == (void*)0x1230ba0) && (g_base_called == 1);

    g_freed = 0; g_base_called = 0;
    CTCInventoryItem b;
    b.vtbl = 0; b.m28 = 0;
    OnDie(&b);
    // m28 null => no delete, vtbl set, base called
    bool ok2 = (g_freed == 0) && (b.vtbl == (void*)0x1230ba0) && (g_base_called == 1);

    if (ok1 && ok2) printf("ONDIE_OK\n");
    else printf("FAIL\n");
    return 0;
}