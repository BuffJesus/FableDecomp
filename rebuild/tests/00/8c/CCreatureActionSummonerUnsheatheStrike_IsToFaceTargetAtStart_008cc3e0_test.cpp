#include <cstdio>
bool __fastcall CCreatureAction_SummonerUnsheatheStrike_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_SummonerUnsheatheStrike_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_008cc3e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cc3e0_TEST FAIL\n");
    return 1;
}