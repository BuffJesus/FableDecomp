#include <cstdio>

struct CTCInventoryItem {
    void* vtbl;
    char  pad[0x24];
    void* obj28;
};

static int g_freed = 0;
static int g_baseCalled = 0;
static void* g_freedPtr = 0;

void EngineFree(void* p) { g_freed++; g_freedPtr = p; }
void BaseOnDie(CTCInventoryItem* self) { g_baseCalled++; }

void OnDie_model(CTCInventoryItem* self)
{
    if (self->obj28)
        EngineFree(self->obj28);
    self->vtbl = (void*)0x01230ba0;
    BaseOnDie(self);
}

int main()
{
    // case 1: obj28 non-null -> free called, vtbl set, base called
    int dummy = 0;
    CTCInventoryItem a;
    a.vtbl = 0;
    a.obj28 = &dummy;
    OnDie_model(&a);
    if (g_freed != 1) { printf("FAIL free\n"); return 1; }
    if (g_freedPtr != &dummy) { printf("FAIL ptr\n"); return 1; }
    if (a.vtbl != (void*)0x01230ba0) { printf("FAIL vtbl\n"); return 1; }
    if (g_baseCalled != 1) { printf("FAIL base\n"); return 1; }

    // case 2: obj28 null -> no free, vtbl set, base called
    CTCInventoryItem b;
    b.vtbl = 0;
    b.obj28 = 0;
    OnDie_model(&b);
    if (g_freed != 1) { printf("FAIL free2\n"); return 1; }
    if (b.vtbl != (void*)0x01230ba0) { printf("FAIL vtbl2\n"); return 1; }
    if (g_baseCalled != 2) { printf("FAIL base2\n"); return 1; }

    printf("BEHAVIOR_OK\n");
    return 0;
}