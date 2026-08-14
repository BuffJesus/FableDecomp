#include <cstdio>

struct CTCInventoryItem
{
    void* vtbl;
    char  pad[0x54];
    void* sub;
};

static int g_freed = 0;
static void* g_freedPtr = 0;
static int g_baseCalled = 0;

void freeThing(void* p) { g_freed++; g_freedPtr = p; }
void BaseOnDie(CTCInventoryItem* self) { g_baseCalled++; }

void OnDie(CTCInventoryItem* self)
{
    if (self->sub)
        freeThing(self->sub);
    self->vtbl = (void*)0x1230ba0;
    BaseOnDie(self);
}

int main()
{
    int dummy;
    CTCInventoryItem a;
    a.vtbl = 0;
    a.sub = &dummy;
    OnDie(&a);
    bool ok1 = (g_freed == 1) && (g_freedPtr == &dummy)
        && (a.vtbl == (void*)0x1230ba0) && (g_baseCalled == 1);

    g_freed = 0; g_freedPtr = 0; g_baseCalled = 0;
    CTCInventoryItem b;
    b.vtbl = 0;
    b.sub = 0;
    OnDie(&b);
    bool ok2 = (g_freed == 0) && (b.vtbl == (void*)0x1230ba0) && (g_baseCalled == 1);

    if (ok1 && ok2)
        printf("ONDIE_OK\n");
    else
        printf("ONDIE_FAIL\n");
    return 0;
}