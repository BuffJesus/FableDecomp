#include <cstdio>

struct CTCInventoryItem {
    void* vtable;
    char  pad[0x24];
    void* member28;
};

static int g_freed = 0;
static void* g_freed_ptr = 0;
static int g_base_called = 0;

void FreeThing(void* p) { g_freed++; g_freed_ptr = p; }
void BaseOnDie(CTCInventoryItem* self) { g_base_called++; }

void OnDie(CTCInventoryItem* self)
{
    if (self->member28)
        FreeThing(self->member28);
    self->vtable = (void*)0x1230ba0;
    BaseOnDie(self);
}

int main()
{
    // case 1: member28 non-null -> frees it
    CTCInventoryItem a;
    a.vtable = 0;
    int dummy = 5;
    a.member28 = &dummy;
    OnDie(&a);
    bool ok1 = (g_freed == 1) && (g_freed_ptr == &dummy)
               && (a.vtable == (void*)0x1230ba0) && (g_base_called == 1);

    // case 2: member28 null -> no free, still sets vtable + base
    CTCInventoryItem b;
    b.vtable = (void*)0xdead;
    b.member28 = 0;
    OnDie(&b);
    bool ok2 = (g_freed == 1) && (b.vtable == (void*)0x1230ba0)
               && (g_base_called == 2);

    if (ok1 && ok2)
        printf("ONDIE_OK\n");
    else
        printf("ONDIE_FAIL\n");
    return 0;
}