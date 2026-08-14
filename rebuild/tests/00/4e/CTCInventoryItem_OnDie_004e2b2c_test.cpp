#include <cstdio>

struct CTCInventoryItem {
    void* vtbl;
    char pad[0x24];
    void* p28;
};

static int g_freed = 0;
static void* g_freed_ptr = 0;
extern "C" void __cdecl free_thing(void* p) { g_freed++; g_freed_ptr = p; }

static int g_base = 0;
static void* g_base_self = 0;
void __fastcall Base_OnDie(void* self) { g_base++; g_base_self = self; }

void __fastcall OnDie(CTCInventoryItem* self)
{
    if (self->p28 != 0)
        free_thing(self->p28);
    self->vtbl = (void*)0x1230ba0;
    Base_OnDie(self);
}

int main()
{
    CTCInventoryItem a;
    a.vtbl = 0; a.p28 = (void*)0x1234;
    OnDie(&a);
    if (g_freed != 1 || g_freed_ptr != (void*)0x1234) { printf("BAD1\n"); return 1; }
    if (a.vtbl != (void*)0x1230ba0) { printf("BAD2\n"); return 1; }
    if (g_base != 1 || g_base_self != &a) { printf("BAD3\n"); return 1; }

    CTCInventoryItem b;
    b.vtbl = 0; b.p28 = 0;
    g_freed = 0; g_base = 0;
    OnDie(&b);
    if (g_freed != 0) { printf("BAD4\n"); return 1; }
    if (b.vtbl != (void*)0x1230ba0) { printf("BAD5\n"); return 1; }
    if (g_base != 1) { printf("BAD6\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}