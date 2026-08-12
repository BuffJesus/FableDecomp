#include <stdio.h>

struct SubObjAt90 {
    void Release();
};

void FreeMapwho(void* p);

struct CTCMapwho {
    char pad[0x90];
    SubObjAt90 sub;
    void OnDie();
};

static int g_release_called = 0;
static int g_free_called = 0;
static void* g_last_sub = 0;
static void* g_last_free = 0;

void SubObjAt90::Release() { g_release_called++; g_last_sub = this; }
void FreeMapwho(void* p) { g_free_called++; g_last_free = p; }

void CTCMapwho::OnDie()
{
    if (this) {
        this->sub.Release();
        FreeMapwho(this);
    }
}

int main()
{
    CTCMapwho m;
    m.OnDie();
    if (g_release_called != 1) { printf("FAIL rel\n"); return 1; }
    if (g_free_called != 1) { printf("FAIL free\n"); return 1; }
    if (g_last_sub != (void*)&m.sub) { printf("FAIL suboff\n"); return 1; }
    if (g_last_free != (void*)&m) { printf("FAIL freearg\n"); return 1; }

    // null this path: emulate by calling through a null pointer
    CTCMapwho* pn = 0;
    pn->OnDie();
    if (g_release_called != 1 || g_free_called != 1) { printf("FAIL null\n"); return 1; }

    printf("MAPWHO_ONDIE_OK\n");
    return 0;
}