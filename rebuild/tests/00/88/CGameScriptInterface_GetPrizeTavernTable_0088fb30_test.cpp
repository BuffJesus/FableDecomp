#include <cstdio>

bool g_PrizeTavernTable = false;

bool __fastcall CGameScriptInterface_GetPrizeTavernTable(void* self)
{
    (void)self;
    return g_PrizeTavernTable;
}

int main()
{
    g_PrizeTavernTable = 1;
    if (!CGameScriptInterface_GetPrizeTavernTable(0)) { std::printf("FAIL true\n"); return 1; }
    g_PrizeTavernTable = 0;
    if (CGameScriptInterface_GetPrizeTavernTable(0)) { std::printf("FAIL false\n"); return 1; }
    std::printf("CGameScriptInterface_0088fb30_TEST PASS\n");
    return 0;
}