#include <cstdio>

bool g_PlayerUsingWillDummies;

bool CGameScriptInterface_GetPlayerUsingWillDummies(void* self)
{
    return g_PlayerUsingWillDummies;
}

int main()
{
    g_PlayerUsingWillDummies = false;
    if (CGameScriptInterface_GetPlayerUsingWillDummies(0)) { std::printf("FAIL zero\n"); return 1; }
    g_PlayerUsingWillDummies = true;
    if (!CGameScriptInterface_GetPlayerUsingWillDummies(0)) { std::printf("FAIL one\n"); return 1; }
    std::printf("CGameScriptInterface_0088fad0_TEST PASS\n");
    return 0;
}