#include <cstdio>

int g_bettingAmount = 0;

void __stdcall CGameScriptInterface_SetBettingAmount(int amount)
{
    g_bettingAmount = amount;
}

int main()
{
    CGameScriptInterface_SetBettingAmount(1337);
    if (g_bettingAmount == 1337) {
        std::printf("CGameScriptInterface_0088fb80_TEST PASS\n");
        return 0;
    }
    std::printf("CGameScriptInterface_0088fb80_TEST FAIL\n");
    return 1;
}