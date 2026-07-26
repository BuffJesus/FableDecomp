#include <cstdio>
struct CGameScriptInterface;
unsigned char g_PlayerUsingRangedDummies = 0;
void __fastcall CGameScriptInterface_SetPlayerUsingRangedDummies(CGameScriptInterface* self, int edx_dummy, unsigned char val)
{
    g_PlayerUsingRangedDummies = val;
}
int main()
{
    CGameScriptInterface_SetPlayerUsingRangedDummies((CGameScriptInterface*)0, 0, 1);
    if (g_PlayerUsingRangedDummies != 1) { std::printf("FAIL set 1\n"); return 1; }
    CGameScriptInterface_SetPlayerUsingRangedDummies((CGameScriptInterface*)0, 0, 0);
    if (g_PlayerUsingRangedDummies != 0) { std::printf("FAIL set 0\n"); return 1; }
    std::printf("CGameScriptInterface_0088faa0_TEST PASS\n");
    return 0;
}