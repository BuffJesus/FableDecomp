#include <cstdio>

bool g_bettingAccept = false;

bool __fastcall CGameScriptInterface_GetBettingAccept(void* self)
{
    (void)self;
    return g_bettingAccept;
}

int main()
{
    g_bettingAccept = false;
    if (CGameScriptInterface_GetBettingAccept(0) != false) { std::printf("FAIL zero\n"); return 1; }
    g_bettingAccept = true;
    if (CGameScriptInterface_GetBettingAccept(0) != true) { std::printf("FAIL one\n"); return 1; }
    std::printf("CGameScriptInterface_0088fb70_TEST PASS\n");
    return 0;
}