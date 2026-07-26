#include <cstdio>
bool __fastcall CCreatureAction_SummonerStrike_IsPartOfComboChain()
{
    return true;
}
int main()
{
    if (CCreatureAction_SummonerStrike_IsPartOfComboChain() == true) { std::printf("AUTO_TINY_006f5bc0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006f5bc0_TEST FAIL\n");
    return 1;
}