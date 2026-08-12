#include <cstdio>

struct CTCMapwho {
    int marker;
    void Cleanup();
};

static int g_cleanup = 0;
static int g_unreg = 0;

void CTCMapwho::Cleanup() { g_cleanup++; }
void MapwhoUnregister(CTCMapwho* p) { if (p) g_unreg++; }

void __stdcall CTCMapwho_OnDie(CTCMapwho* p)
{
    if (p) {
        p->Cleanup();
        MapwhoUnregister(p);
    }
}

int main()
{
    // null: nothing happens
    CTCMapwho_OnDie(0);
    if (g_cleanup != 0 || g_unreg != 0) { printf("FAIL null\n"); return 1; }

    CTCMapwho obj; obj.marker = 7;
    CTCMapwho_OnDie(&obj);
    if (g_cleanup != 1 || g_unreg != 1) { printf("FAIL call\n"); return 1; }

    printf("ONDIE_OK\n");
    return 0;
}