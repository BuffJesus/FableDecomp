#include <cstdio>
bool __fastcall CCreatureAction_BattleChargeMiss_IsToCauseRecoil()
{
    return true;
}
int main()
{
    if (CCreatureAction_BattleChargeMiss_IsToCauseRecoil() == true) { std::printf("AUTO_TINY_008cec90_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cec90_TEST FAIL\n");
    return 1;
}