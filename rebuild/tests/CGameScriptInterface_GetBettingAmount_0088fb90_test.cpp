#include <cstdio>

struct CGameScriptInterface;
long g_bettingAmount = 0;

long __fastcall CGameScriptInterface_GetBettingAmount(CGameScriptInterface* self)
{
    return g_bettingAmount;
}

int main()
{
    g_bettingAmount = 1234;
    long r = CGameScriptInterface_GetBettingAmount((CGameScriptInterface*)0);
    if (r != 1234) { std::printf("FAIL got %ld\n", r); return 1; }
    g_bettingAmount = -7;
    r = CGameScriptInterface_GetBettingAmount((CGameScriptInterface*)0);
    if (r != -7) { std::printf("FAIL got %ld\n", r); return 1; }
    std::printf("CGameScriptInterface_0088fb90_TEST PASS\n");
    return 0;
}