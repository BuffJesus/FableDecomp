#include <cstdio>

struct CTCInventoryItem {
    void* vtbl;
    char pad[0x40-4];
    void* m40;
};

static int g_freed = 0;
static int g_tailed = 0;
static void* g_freedptr = 0;

extern "C" void sub_bfea14(void* p) { g_freed++; g_freedptr = p; }
void sub_9fc550(CTCInventoryItem* self) { g_tailed++; }

void OnDie(CTCInventoryItem* self)
{
    if (self->m40)
        sub_bfea14(self->m40);
    self->vtbl = (void*)0x1230ba0;
    sub_9fc550(self);
}

int main()
{
    // case 1: m40 non-null -> free called
    CTCInventoryItem a;
    int dummy = 0;
    a.m40 = &dummy;
    OnDie(&a);
    if (g_freed != 1 || g_freedptr != &dummy) { printf("FAIL free\n"); return 1; }
    if (a.vtbl != (void*)0x1230ba0) { printf("FAIL vtbl\n"); return 1; }
    if (g_tailed != 1) { printf("FAIL tail\n"); return 1; }

    // case 2: m40 null -> no free
    CTCInventoryItem b;
    b.m40 = 0;
    OnDie(&b);
    if (g_freed != 1) { printf("FAIL nofree\n"); return 1; }
    if (b.vtbl != (void*)0x1230ba0) { printf("FAIL vtbl2\n"); return 1; }
    if (g_tailed != 2) { printf("FAIL tail2\n"); return 1; }

    printf("ONDIE_OK\n");
    return 0;
}