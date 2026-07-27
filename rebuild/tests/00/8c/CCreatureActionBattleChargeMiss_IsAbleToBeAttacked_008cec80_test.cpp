#include <cstdio>
bool __fastcall CCreatureAction_BattleChargeMiss_IsAbleToBeAttacked()
{
    return false;
}
int main()
{
    if (CCreatureAction_BattleChargeMiss_IsAbleToBeAttacked() == false) { std::printf("AUTO_TINY_008cec80_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cec80_TEST FAIL\n");
    return 1;
}