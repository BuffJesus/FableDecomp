#include <cstdio>

struct CTCInventoryItem {
    unsigned char _pad_0x0[0x10];
    void* ptr10;
    unsigned char _pad_0x14[0xc];
    char sub20;
    unsigned char _pad_0x21[0x5f];
    void* ptr80;
};

static int g_freed = 0;
static int g_based = 0;
static void* g_freedptr = 0;

extern "C" void __cdecl FreeThing(void* p) { g_freed++; g_freedptr = p; }
void __fastcall Base_OnDie(CTCInventoryItem* self) { g_based++; }

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    if (self->ptr80)
        FreeThing(self->ptr80);
    Base_OnDie(self);
}

int main()
{
    CTCInventoryItem a;
    a.ptr80 = (void*)0x1234;
    CTCInventoryItem_OnDie(&a);
    if (g_freed != 1 || g_freedptr != (void*)0x1234 || g_based != 1) {
        std::printf("FAIL nonnull path\n");
        return 1;
    }
    g_freed = 0; g_based = 0;
    CTCInventoryItem b;
    b.ptr80 = 0;
    CTCInventoryItem_OnDie(&b);
    if (g_freed != 0 || g_based != 1) {
        std::printf("FAIL null path\n");
        return 1;
    }
    std::printf("CTCInventoryItem_004570b7_TEST PASS\n");
    return 0;
}