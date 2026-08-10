#include <cstdio>

struct CTCMapwho { int tag; };

static int g_method = 0;
static int g_free = 0;
static CTCMapwho* g_order[4];
static int g_n = 0;

void __fastcall CTCMapwho_Method(CTCMapwho* self){ g_method++; g_order[g_n++]=self; }
void CTCMapwho_FreeFn(CTCMapwho* p){ g_free++; g_order[g_n++]=p; }

void __fastcall OnDie(CTCMapwho* self);

int main()
{
    // null: nothing happens
    OnDie(0);
    if (g_method!=0 || g_free!=0) { printf("FAIL null\n"); return 1; }

    CTCMapwho obj; obj.tag=7;
    OnDie(&obj);
    if (g_method!=1 || g_free!=1) { printf("FAIL counts\n"); return 1; }
    // method called before free, both with this
    if (g_order[0]!=&obj || g_order[1]!=&obj) { printf("FAIL args/order\n"); return 1; }

    printf("00aecca0_TEST PASS\n");
    return 0;
}