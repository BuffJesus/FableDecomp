#include <cstdio>

struct CTCInventoryItem {
    char pad[0x10];
    void* f10;
};

static int g_freed = 0;
static void* g_freedPtr = 0;
static int g_base = 0;
static CTCInventoryItem* g_baseSelf = 0;

extern "C" void __cdecl free_bfea14(void* p) { g_freed++; g_freedPtr = p; }
void __fastcall base_686830(CTCInventoryItem* self) { g_base++; g_baseSelf = self; }

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    if (self->f10)
        free_bfea14(self->f10);
    base_686830(self);
}

int main()
{
    CTCInventoryItem a;
    for (int i = 0; i < 0x10; ++i) a.pad[i] = 0;
    int dummy = 0;
    a.f10 = &dummy;
    CTCInventoryItem_OnDie(&a);
    bool ok1 = (g_freed == 1) && (g_freedPtr == &dummy) && (g_base == 1) && (g_baseSelf == &a);

    g_freed = 0; g_freedPtr = 0; g_base = 0; g_baseSelf = 0;
    CTCInventoryItem b;
    for (int i = 0; i < 0x10; ++i) b.pad[i] = 0;
    b.f10 = 0;
    CTCInventoryItem_OnDie(&b);
    bool ok2 = (g_freed == 0) && (g_base == 1) && (g_baseSelf == &b);

    if (ok1 && ok2)
        printf("004de60a_TEST PASS\n");
    else
        printf("FAIL %d %d\n", (int)ok1, (int)ok2);
    return 0;
}