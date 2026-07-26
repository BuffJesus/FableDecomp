#include <cstdio>

unsigned char g_quitTavernGame = 0;

void __stdcall CGameScriptInterface_SetQuitTavernGame(bool b)
{
    g_quitTavernGame = (unsigned char)b;
}

int main()
{
    CGameScriptInterface_SetQuitTavernGame(true);
    if (g_quitTavernGame != 1) { std::printf("FAIL set true\n"); return 1; }
    CGameScriptInterface_SetQuitTavernGame(false);
    if (g_quitTavernGame != 0) { std::printf("FAIL set false\n"); return 1; }
    std::printf("CGameScriptInterface_0088fb00_TEST PASS\n");
    return 0;
}