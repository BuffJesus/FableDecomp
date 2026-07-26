#include <cstdio>
struct CTCMapwho { int tag; };
static int g_cleanup = 0;
static int g_free = 0;
static CTCMapwho* g_freed = 0;

extern "C" void __fastcall CTCMapwho_Cleanup(CTCMapwho* self) { g_cleanup++; }
extern "C" void FreeMapwho(CTCMapwho* self) { g_free++; g_freed = self; }

void __fastcall CTCMapwho_OnDie(CTCMapwho* self)
{
    if (self)
    {
        CTCMapwho_Cleanup(self);
        FreeMapwho(self);
    }
}

int main()
{
    CTCMapwho obj; obj.tag = 7;
    CTCMapwho_OnDie(&obj);
    if (g_cleanup != 1 || g_free != 1 || g_freed != &obj) { std::printf("FAIL non-null path\n"); return 1; }
    g_cleanup = g_free = 0; g_freed = 0;
    CTCMapwho_OnDie(0);
    if (g_cleanup != 0 || g_free != 0) { std::printf("FAIL null path\n"); return 1; }
    std::printf("CTCMapwho_00444d70_TEST PASS\n");
    return 0;
}