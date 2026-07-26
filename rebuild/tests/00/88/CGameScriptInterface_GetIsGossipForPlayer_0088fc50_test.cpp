#include <cstdio>
int __fastcall CGameScriptInterface_GetIsGossipForPlayer()
{
    return 20688440;
}
int main()
{
    if (CGameScriptInterface_GetIsGossipForPlayer() == 20688440) { std::printf("AUTO_TINY_0088fc50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0088fc50_TEST FAIL\n");
    return 1;
}