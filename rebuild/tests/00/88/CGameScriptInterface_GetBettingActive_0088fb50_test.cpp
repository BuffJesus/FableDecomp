#include <cstdio>

bool g_bettingActive = false;

bool __fastcall CGameScriptInterface_GetBettingActive(void* self)
{
    (void)self;
    return g_bettingActive;
}

int main()
{
    g_bettingActive = false;
    if (CGameScriptInterface_GetBettingActive(0) != false) { std::printf("FAIL zero\n"); return 1; }
    g_bettingActive = true;
    if (CGameScriptInterface_GetBettingActive(0) != true) { std::printf("FAIL one\n"); return 1; }
    std::printf("CGameScriptInterface_0088fb50_TEST PASS\n");
    return 0;
}