#include <cstdio>

struct CGameScriptInterface;
unsigned char g_countBetMoneyDown = 0;

void __fastcall CGameScriptInterface_SetCountBetMoneyDown(CGameScriptInterface* self, void* edx, bool v)
{
    (void)self;
    (void)edx;
    g_countBetMoneyDown = (unsigned char)v;
}

int main()
{
    CGameScriptInterface_SetCountBetMoneyDown((CGameScriptInterface*)0, 0, true);
    if (g_countBetMoneyDown != 1) { std::printf("FAIL set true\n"); return 1; }
    CGameScriptInterface_SetCountBetMoneyDown((CGameScriptInterface*)0, 0, false);
    if (g_countBetMoneyDown != 0) { std::printf("FAIL set false\n"); return 1; }
    std::printf("CGameScriptInterface_0088fba0_TEST PASS\n");
    return 0;
}