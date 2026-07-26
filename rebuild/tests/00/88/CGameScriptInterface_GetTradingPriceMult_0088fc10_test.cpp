#include <cstdio>

struct CGameScriptInterface;
float g_TradingPriceMult = 2.5f;

float __fastcall CGameScriptInterface_GetTradingPriceMult(const CGameScriptInterface* self)
{
    (void)self;
    return g_TradingPriceMult;
}

int main()
{
    g_TradingPriceMult = 3.25f;
    float r = CGameScriptInterface_GetTradingPriceMult((const CGameScriptInterface*)0x1000);
    if (r == 3.25f) {
        std::printf("CGameScriptInterface_0088fc10_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %f\n", r);
    return 1;
}