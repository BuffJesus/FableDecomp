#include <cstdio>

struct CTCInventoryItem {
    char pad[0x3c];
    void* f3c;
};

static int g_freed = 0;
static void* g_freed_ptr = 0;
static int g_base_called = 0;

extern "C" void __cdecl free_bfea14(void* p) { g_freed++; g_freed_ptr = p; }
void __fastcall base_430300(CTCInventoryItem* self) { g_base_called++; (void)self; }

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self);

int main()
{
    CTCInventoryItem a;
    a.f3c = (void*)0xdeadbeef;
    CTCInventoryItem_OnDie(&a);
    bool ok1 = (g_freed == 1) && (g_freed_ptr == (void*)0xdeadbeef) && (g_base_called == 1);

    CTCInventoryItem b;
    b.f3c = 0;
    CTCInventoryItem_OnDie(&b);
    bool ok2 = (g_freed == 1) && (g_base_called == 2);

    if (ok1 && ok2)
        printf("004f9cfc_TEST PASS\n");
    else
        printf("FAIL f=%d bp=%p bc=%d\n", g_freed, g_freed_ptr, g_base_called);
    return 0;
}