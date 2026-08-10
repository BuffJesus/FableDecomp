#include <stdio.h>

struct CTCMapwho { int dummy; };

static int g_dtor = 0;
static int g_free = 0;
static void* g_freed = 0;

extern "C" void __fastcall Mapwho_Dtor(CTCMapwho* self) { g_dtor++; }
extern "C" void __cdecl Mapwho_Free(void* p) { g_free++; g_freed = p; }

void __fastcall OnDie(CTCMapwho* self);

int main()
{
    CTCMapwho obj; obj.dummy = 7;
    OnDie(&obj);
    if (g_dtor != 1 || g_free != 1 || g_freed != &obj) { printf("FAIL a\n"); return 1; }
    g_dtor = g_free = 0; g_freed = 0;
    OnDie(0);
    if (g_dtor != 0 || g_free != 0) { printf("FAIL b\n"); return 1; }
    printf("OK_006baa70\n");
    return 0;
}