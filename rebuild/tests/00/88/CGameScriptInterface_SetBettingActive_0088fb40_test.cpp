#include <cstdio>
unsigned char g_bettingActive = 0;
void __stdcall CGameScriptInterface_SetBettingActive(bool active)
{
    g_bettingActive = (unsigned char)active;
}
int main()
{
    CGameScriptInterface_SetBettingActive(true);
    if (g_bettingActive != 1) { std::printf("FAIL set true\n"); return 1; }
    CGameScriptInterface_SetBettingActive(false);
    if (g_bettingActive != 0) { std::printf("FAIL set false\n"); return 1; }
    std::printf("CGameScriptInterface_0088fb40_TEST PASS\n");
    return 0;
}