#include <cstdio>
struct CGameScriptInterface;
bool g_countBetMoneyDown = false;
bool __fastcall CGameScriptInterface_GetCountBetMoneyDown(const CGameScriptInterface* self)
{
    return g_countBetMoneyDown;
}
int main()
{
    g_countBetMoneyDown = false;
    if (CGameScriptInterface_GetCountBetMoneyDown(0) != false) { std::printf("FAIL zero\n"); return 1; }
    g_countBetMoneyDown = true;
    if (CGameScriptInterface_GetCountBetMoneyDown(0) != true) { std::printf("FAIL one\n"); return 1; }
    std::printf("CGameScriptInterface_0088fbb0_TEST PASS\n");
    return 0;
}