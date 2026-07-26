#include <cstdio>
int __fastcall CGameScriptInterface_GetActiveGossipCategories()
{
    return 20688428;
}
int main()
{
    if (CGameScriptInterface_GetActiveGossipCategories() == 20688428) { std::printf("AUTO_TINY_0088fc40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0088fc40_TEST FAIL\n");
    return 1;
}