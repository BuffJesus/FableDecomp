#include <cstdio>
bool __fastcall CCreatureAction_BattleChargeImpact_IsToCauseRecoil()
{
    return true;
}
int main()
{
    if (CCreatureAction_BattleChargeImpact_IsToCauseRecoil() == true) { std::printf("AUTO_TINY_008cec60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cec60_TEST FAIL\n");
    return 1;
}