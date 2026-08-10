#include <cstdio>

struct CTCMapwho { int tag; };

static int g_inner = 0;
static int g_free = 0;
static CTCMapwho *g_seen_inner = 0;
static CTCMapwho *g_seen_free = 0;

void __fastcall CTCMapwho_inner(CTCMapwho *self){ g_inner++; g_seen_inner=self; }
void CTCMapwho_free(CTCMapwho *p){ g_free++; g_seen_free=p; }

void __fastcall CTCMapwho_OnDie(CTCMapwho *self);

int main()
{
    // null: nothing happens
    CTCMapwho_OnDie(0);
    if (g_inner!=0 || g_free!=0) { printf("FAIL null\n"); return 1; }

    CTCMapwho obj; obj.tag=7;
    CTCMapwho_OnDie(&obj);
    if (g_inner==1 && g_free==1 && g_seen_inner==&obj && g_seen_free==&obj)
        printf("00a47040_TEST PASS\n");
    else
        printf("FAIL\n");
    return 0;
}