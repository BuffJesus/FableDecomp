#include <cstdio>

struct CTCInventoryItem {
    void* vptr;
    unsigned char _pad4[0x94];
    void* ptr98; // +0x98
};

static int g_freed = 0;
static void* g_last = 0;
static int g_base = 0;

extern "C" void freemem(void* p) { g_freed++; g_last = p; }
void __fastcall Base_OnDie(CTCInventoryItem* self) { (void)self; g_base++; }

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    void* p = self->ptr98;
    if (p != 0)
        freemem(p);
    Base_OnDie(self);
}

int main()
{
    CTCInventoryItem a;
    int dummy = 5;
    a.ptr98 = &dummy;
    CTCInventoryItem_OnDie(&a);
    if (g_freed != 1 || g_last != &dummy || g_base != 1) { std::printf("FAIL nonnull\n"); return 1; }

    g_freed = 0; g_base = 0; g_last = 0;
    CTCInventoryItem b;
    b.ptr98 = 0;
    CTCInventoryItem_OnDie(&b);
    if (g_freed != 0 || g_base != 1) { std::printf("FAIL null\n"); return 1; }

    std::printf("CTCInventoryItem_0045775a_TEST PASS\n");
    return 0;
}