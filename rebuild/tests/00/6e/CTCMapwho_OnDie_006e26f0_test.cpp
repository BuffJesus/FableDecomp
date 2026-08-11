#include <stdio.h>
#include <stdlib.h>

struct Sub28 {
    int pad;
    void Cleanup();
};

struct CTCMapwho {
    char pad0[0x28];
    Sub28 sub;
    void OnDie();
};

static int g_cleaned = 0;
static int g_freed = 0;

void Sub28::Cleanup() { g_cleaned++; }

extern "C" void __cdecl free_it(void* p) { g_freed++; free(p); }

int main()
{
    // null path
    CTCMapwho* n = 0;
    n->OnDie();
    if (g_cleaned != 0 || g_freed != 0) { printf("FAIL null\n"); return 1; }

    // non-null path
    CTCMapwho* p = (CTCMapwho*)malloc(sizeof(CTCMapwho));
    p->OnDie();
    if (g_cleaned != 1 || g_freed != 1) { printf("FAIL live\n"); return 1; }

    printf("ONDIE_OK\n");
    return 0;
}