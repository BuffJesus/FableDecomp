#include <cstdio>

void* g_pGameDefinitionManager = 0;

bool __fastcall CGameDefinitionManager_IsAllocated()
{
    return g_pGameDefinitionManager != 0;
}

int main()
{
    g_pGameDefinitionManager = 0;
    if (CGameDefinitionManager_IsAllocated() != false) { std::printf("FAIL null\n"); return 1; }
    int dummy = 42;
    g_pGameDefinitionManager = &dummy;
    if (CGameDefinitionManager_IsAllocated() != true) { std::printf("FAIL nonnull\n"); return 1; }
    std::printf("_global_0044c6b6_TEST PASS\n");
    return 0;
}