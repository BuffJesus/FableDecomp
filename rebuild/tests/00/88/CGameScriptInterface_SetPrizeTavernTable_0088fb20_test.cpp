#include <cstdio>

unsigned char g_prizeTavernTable = 0;

void __stdcall CGameScriptInterface_SetPrizeTavernTable(bool enabled)
{
    g_prizeTavernTable = (unsigned char)enabled;
}

int main()
{
    CGameScriptInterface_SetPrizeTavernTable(true);
    if (g_prizeTavernTable != 1) { std::printf("FAIL: expected 1 got %d\n", (int)g_prizeTavernTable); return 1; }
    CGameScriptInterface_SetPrizeTavernTable(false);
    if (g_prizeTavernTable != 0) { std::printf("FAIL: expected 0 got %d\n", (int)g_prizeTavernTable); return 1; }
    std::printf("CGameScriptInterface_0088fb20_TEST PASS\n");
    return 0;
}