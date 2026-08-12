#include <cstdio>

struct CTCMapwho {
    int calls;
    void Shutdown();
};

static CTCMapwho* g_last_freed = 0;
static int g_shutdown_count = 0;

void CTCMapwho::Shutdown() { this->calls++; g_shutdown_count++; }

void FreeThing(CTCMapwho* p) { g_last_freed = p; }

void __stdcall OnDie(CTCMapwho* p)
{
    if (p) {
        p->Shutdown();
        FreeThing(p);
    }
}

int main()
{
    // null path: no crash, no side effects
    OnDie(0);
    if (g_shutdown_count != 0 || g_last_freed != 0) { printf("FAIL null\n"); return 1; }

    CTCMapwho obj;
    obj.calls = 0;
    OnDie(&obj);
    if (g_shutdown_count != 1) { printf("FAIL shutdown\n"); return 1; }
    if (obj.calls != 1) { printf("FAIL member\n"); return 1; }
    if (g_last_freed != &obj) { printf("FAIL free\n"); return 1; }

    printf("ONDIE_OK\n");
    return 0;
}