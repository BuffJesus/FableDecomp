#include <cstdio>

struct CTCMapwho { int x; };

static int g_helper = 0;
static CTCMapwho* g_freed = 0;

extern "C" void helper_9f9020(void) { g_helper++; }
extern "C" void free_bfe9bc(CTCMapwho* p) { g_freed = p; }

void __fastcall OnDie(CTCMapwho* self);

int main()
{
    OnDie(0);
    if (g_helper != 0 || g_freed != 0) { printf("FAIL null path\n"); return 1; }

    CTCMapwho obj; obj.x = 5;
    OnDie(&obj);
    if (g_helper != 1) { printf("FAIL helper\n"); return 1; }
    if (g_freed != &obj) { printf("FAIL free\n"); return 1; }

    printf("009a73b0_TEST PASS\n");
    return 0;
}