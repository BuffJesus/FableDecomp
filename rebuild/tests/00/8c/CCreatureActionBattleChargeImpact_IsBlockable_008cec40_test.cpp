#include <cstdio>
bool __fastcall CCreatureAction_BattleChargeImpact_IsBlockable()
{
    return false;
}
int main()
{
    if (CCreatureAction_BattleChargeImpact_IsBlockable() == false) { std::printf("AUTO_TINY_008cec40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cec40_TEST FAIL\n");
    return 1;
}