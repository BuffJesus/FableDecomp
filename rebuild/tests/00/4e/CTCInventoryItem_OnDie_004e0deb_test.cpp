#include <cstdio>

typedef unsigned long DWORD;

struct CTCInventoryItem {
    DWORD* vtable;
    char pad[0x28 - 4];
    void* p28;
};

static int g_freed = 0;
static void* g_freed_arg = 0;
static int g_base_called = 0;
static CTCInventoryItem* g_base_self = 0;

void __cdecl free_helper(void* p) { g_freed++; g_freed_arg = p; }
void __fastcall base_OnDie(CTCInventoryItem* self) { g_base_called++; g_base_self = self; }

void __fastcall OnDie(CTCInventoryItem* self)
{
    void* p = self->p28;
    if (p) {
        free_helper(p);
    }
    self->vtable = (DWORD*)0x1230ba0;
    base_OnDie(self);
}

int main()
{
    // Case 1: p28 non-null -> free called
    CTCInventoryItem a;
    a.vtable = 0;
    a.p28 = (void*)0xdead;
    OnDie(&a);
    if (g_freed != 1 || g_freed_arg != (void*)0xdead) { printf("FAIL free\n"); return 1; }
    if (a.vtable != (DWORD*)0x1230ba0) { printf("FAIL vtable1\n"); return 1; }
    if (g_base_called != 1 || g_base_self != &a) { printf("FAIL base1\n"); return 1; }

    // Case 2: p28 null -> free NOT called
    CTCInventoryItem b;
    b.vtable = (DWORD*)0x99;
    b.p28 = 0;
    OnDie(&b);
    if (g_freed != 1) { printf("FAIL free-skip\n"); return 1; }
    if (b.vtable != (DWORD*)0x1230ba0) { printf("FAIL vtable2\n"); return 1; }
    if (g_base_called != 2 || g_base_self != &b) { printf("FAIL base2\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}