#include <cstdio>

bool g_QuitTavernGame = false;

struct CGameScriptInterface { char dummy; };

bool __fastcall CGameScriptInterface_GetQuitTavernGame(CGameScriptInterface* self)
{
    (void)self;
    return g_QuitTavernGame;
}

int main()
{
    CGameScriptInterface obj;
    g_QuitTavernGame = false;
    if (CGameScriptInterface_GetQuitTavernGame(&obj) != false) { std::printf("FAIL zero\n"); return 1; }
    g_QuitTavernGame = true;
    if (CGameScriptInterface_GetQuitTavernGame(&obj) != true) { std::printf("FAIL one\n"); return 1; }
    std::printf("CGameScriptInterface_0088fb10_TEST PASS\n");
    return 0;
}