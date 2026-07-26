#include <cstdio>
bool g_playerUsingRangedDummies;
struct CGameScriptInterface { int _dummy; };
bool __fastcall CGameScriptInterface_GetPlayerUsingRangedDummies(CGameScriptInterface* self)
{
    return g_playerUsingRangedDummies;
}
int main()
{
    CGameScriptInterface obj;
    g_playerUsingRangedDummies = true;
    if (CGameScriptInterface_GetPlayerUsingRangedDummies(&obj) != true) { std::printf("FAIL true case\n"); return 1; }
    g_playerUsingRangedDummies = false;
    if (CGameScriptInterface_GetPlayerUsingRangedDummies(&obj) != false) { std::printf("FAIL false case\n"); return 1; }
    std::printf("CGameScriptInterface_0088fab0_TEST PASS\n");
    return 0;
}