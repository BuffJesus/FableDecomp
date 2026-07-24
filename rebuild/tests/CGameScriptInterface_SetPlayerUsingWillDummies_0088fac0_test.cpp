#include <cstdio>

unsigned char g_playerUsingWillDummies = 0;

void __stdcall CGameScriptInterface_SetPlayerUsingWillDummies(bool bUsing)
{
    g_playerUsingWillDummies = (unsigned char)bUsing;
}

int main()
{
    CGameScriptInterface_SetPlayerUsingWillDummies(true);
    if (g_playerUsingWillDummies != 1) { std::printf("FAIL: expected 1 got %d\n", (int)g_playerUsingWillDummies); return 1; }
    CGameScriptInterface_SetPlayerUsingWillDummies(false);
    if (g_playerUsingWillDummies != 0) { std::printf("FAIL: expected 0 got %d\n", (int)g_playerUsingWillDummies); return 1; }
    std::printf("CGameScriptInterface_0088fac0_TEST PASS\n");
    return 0;
}