#include <cstdio>

extern "C" void __cdecl FreeThing(void* p);

struct BaseItem { void OnDie(); };
struct CTCInventoryItem : public BaseItem {
    char pad00[0x2c];
    void* ptr2c;
    void OnDie();
};

static int g_freed = 0;
static void* g_lastFreed = 0;
static int g_baseCalled = 0;

extern "C" void __cdecl FreeThing(void* p) { g_freed++; g_lastFreed = p; }
void BaseItem::OnDie() { g_baseCalled++; }

int main()
{
    // Case 1: ptr2c non-null -> FreeThing called, base called
    CTCInventoryItem a;
    int dummy = 0;
    a.ptr2c = &dummy;
    a.OnDie();
    if (g_freed != 1 || g_lastFreed != &dummy || g_baseCalled != 1) { printf("FAIL1\n"); return 1; }

    // Case 2: ptr2c null -> FreeThing NOT called, base called
    CTCInventoryItem b;
    b.ptr2c = 0;
    b.OnDie();
    if (g_freed != 1 || g_baseCalled != 2) { printf("FAIL2\n"); return 1; }

    printf("ONDIE_OK\n");
    return 0;
}