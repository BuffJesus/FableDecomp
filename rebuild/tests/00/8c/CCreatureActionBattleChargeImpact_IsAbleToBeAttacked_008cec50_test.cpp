#include <cstdio>
bool __fastcall CCreatureAction_BattleChargeImpact_IsAbleToBeAttacked()
{
    return false;
}
int main()
{
    if (CCreatureAction_BattleChargeImpact_IsAbleToBeAttacked() == false) { std::printf("AUTO_TINY_008cec50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cec50_TEST FAIL\n");
    return 1;
}