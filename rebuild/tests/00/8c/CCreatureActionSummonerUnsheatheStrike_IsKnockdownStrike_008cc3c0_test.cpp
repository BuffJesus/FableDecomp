#include <cstdio>
bool __fastcall CCreatureAction_SummonerUnsheatheStrike_IsKnockdownStrike()
{
    return true;
}
int main()
{
    if (CCreatureAction_SummonerUnsheatheStrike_IsKnockdownStrike() == true) { std::printf("AUTO_TINY_008cc3c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cc3c0_TEST FAIL\n");
    return 1;
}