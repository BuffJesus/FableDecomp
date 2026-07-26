#include <cstdio>
bool __fastcall CCreatureAction_ScorpionChargeStrike_IsKnockdownStrike()
{
    return true;
}
int main()
{
    if (CCreatureAction_ScorpionChargeStrike_IsKnockdownStrike() == true) { std::printf("AUTO_TINY_008c5c10_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c5c10_TEST FAIL\n");
    return 1;
}