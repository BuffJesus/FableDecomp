#include <cstdio>

struct CTCMapwho { int tag; };

static int g_cleanup = 0;
static int g_free = 0;
static CTCMapwho* g_last = 0;

void __fastcall CTCMapwho_Cleanup(CTCMapwho* self) { g_cleanup++; g_last = self; }
void CTCMapwho_Free(void* p) { g_free++; }

void __fastcall CTCMapwho_OnDie(CTCMapwho* self)
{
    if (self)
    {
        CTCMapwho_Cleanup(self);
        CTCMapwho_Free(self);
    }
}

int main()
{
    CTCMapwho obj; obj.tag = 7;
    CTCMapwho_OnDie(&obj);
    // null case: nothing happens
    CTCMapwho_OnDie(0);

    if (g_cleanup == 1 && g_free == 1 && g_last == &obj)
        printf("00aecbf0_TEST PASS\n");
    else
        printf("FAIL c=%d f=%d\n", g_cleanup, g_free);
    return 0;
}