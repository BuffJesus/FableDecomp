#include <cstdio>
bool __fastcall CCreatureAction_SummonerUnsheatheStrike_IsBlockable()
{
    return false;
}
int main()
{
    if (CCreatureAction_SummonerUnsheatheStrike_IsBlockable() == false) { std::printf("AUTO_TINY_008cc3d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cc3d0_TEST FAIL\n");
    return 1;
}