#include <cstdio>

struct CTCInventoryItem
{
    char pad[0x3c];
    void* field3c;
};

static int g_freed = 0;
static int g_base = 0;
static void* g_freed_ptr = 0;

extern "C" void __cdecl InvItem_Free(void* p) { g_freed = 1; g_freed_ptr = p; }
extern "C" void __fastcall InvItem_BaseOnDie(CTCInventoryItem* self) { g_base = 1; }

void __fastcall OnDie(CTCInventoryItem* self)
{
    void* p = self->field3c;
    if (p)
        InvItem_Free(p);
    InvItem_BaseOnDie(self);
}

int main()
{
    CTCInventoryItem a;
    int dummy = 5;
    a.field3c = &dummy;
    g_freed = 0; g_base = 0; g_freed_ptr = 0;
    OnDie(&a);
    if (!(g_freed == 1 && g_freed_ptr == &dummy && g_base == 1)) { printf("FAIL1\n"); return 1; }

    CTCInventoryItem b;
    b.field3c = 0;
    g_freed = 0; g_base = 0;
    OnDie(&b);
    if (!(g_freed == 0 && g_base == 1)) { printf("FAIL2\n"); return 1; }

    printf("0047955b_TEST PASS\n");
    return 0;
}