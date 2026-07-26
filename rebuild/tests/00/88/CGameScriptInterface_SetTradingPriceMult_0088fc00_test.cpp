#include <cstdio>

int g_tradingPriceMult;

void __stdcall CGameScriptInterface_SetTradingPriceMult(int mult)
{
    g_tradingPriceMult = mult;
}

int main()
{
    g_tradingPriceMult = 0;
    CGameScriptInterface_SetTradingPriceMult(42);
    if (g_tradingPriceMult != 42) { std::printf("FAIL: value=%d\n", g_tradingPriceMult); return 1; }
    CGameScriptInterface_SetTradingPriceMult(-7);
    if (g_tradingPriceMult != -7) { std::printf("FAIL: value=%d\n", g_tradingPriceMult); return 1; }
    std::printf("CGameScriptInterface_0088fc00_TEST PASS\n");
    return 0;
}