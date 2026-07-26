#include <cstdio>

struct CTCInventoryItem {
    void* vptr;      // +0x0
    char _pad4[0x40];
    void* ptr44;     // +0x44
};

static int g_freed = 0;
static void* g_freed_ptr = 0;
static int g_base_called = 0;
static CTCInventoryItem* g_base_self = 0;

extern "C" void __cdecl eng_free(void* p) { g_freed++; g_freed_ptr = p; }
void __fastcall CTCInventoryItem_Base_OnDie(CTCInventoryItem* self) { g_base_called++; g_base_self = self; }

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    void* p = self->ptr44;
    if (p != 0)
        eng_free(p);
    CTCInventoryItem_Base_OnDie(self);
}

int main()
{
    CTCInventoryItem a;
    int dummy = 0;
    a.ptr44 = &dummy;
    g_freed = 0; g_base_called = 0;
    CTCInventoryItem_OnDie(&a);
    if (g_freed != 1 || g_freed_ptr != &dummy || g_base_called != 1 || g_base_self != &a) {
        std::printf("FAIL: nonnull path freed=%d base=%d\n", g_freed, g_base_called);
        return 1;
    }

    CTCInventoryItem b;
    b.ptr44 = 0;
    g_freed = 0; g_base_called = 0;
    CTCInventoryItem_OnDie(&b);
    if (g_freed != 0 || g_base_called != 1 || g_base_self != &b) {
        std::printf("FAIL: null path freed=%d base=%d\n", g_freed, g_base_called);
        return 1;
    }

    std::printf("CTCInventoryItem_00457b72_TEST PASS\n");
    return 0;
}