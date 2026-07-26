#include <cstdio>
struct CGameScriptInterface { char pad[4]; };
bool g_PlayerUsingMeleeDummies = false;
bool __fastcall CGameScriptInterface_GetPlayerUsingMeleeDummies(const CGameScriptInterface* self)
{
    (void)self;
    return g_PlayerUsingMeleeDummies;
}
int main()
{
    CGameScriptInterface obj;
    g_PlayerUsingMeleeDummies = 0;
    if (CGameScriptInterface_GetPlayerUsingMeleeDummies(&obj)) { std::printf("FAIL: expected false\n"); return 1; }
    g_PlayerUsingMeleeDummies = 1;
    if (!CGameScriptInterface_GetPlayerUsingMeleeDummies(&obj)) { std::printf("FAIL: expected true\n"); return 1; }
    std::printf("CGameScriptInterface_0088fa90_TEST PASS\n");
    return 0;
}