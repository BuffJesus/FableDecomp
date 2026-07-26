#include <cstdio>

unsigned char g_playerUsingMeleeDummies;
void __stdcall CGameScriptInterface_SetPlayerUsingMeleeDummies(unsigned char enabled)
{
    g_playerUsingMeleeDummies = enabled;
}

int main()
{
    g_playerUsingMeleeDummies = 0;
    CGameScriptInterface_SetPlayerUsingMeleeDummies(1);
    if (g_playerUsingMeleeDummies != 1) { std::printf("FAIL set 1\n"); return 1; }
    CGameScriptInterface_SetPlayerUsingMeleeDummies(0);
    if (g_playerUsingMeleeDummies != 0) { std::printf("FAIL set 0\n"); return 1; }
    CGameScriptInterface_SetPlayerUsingMeleeDummies(0xAB);
    if (g_playerUsingMeleeDummies != 0xAB) { std::printf("FAIL set AB\n"); return 1; }
    std::printf("CGameScriptInterface_0088fa80_TEST PASS\n");
    return 0;
}