#include <cstdio>
bool __fastcall CCreatureAction_ScorpionUnblockableLeftTurnAttack_IsKnockdownStrike()
{
    return true;
}
int main()
{
    if (CCreatureAction_ScorpionUnblockableLeftTurnAttack_IsKnockdownStrike() == true) { std::printf("AUTO_TINY_0097c7f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0097c7f0_TEST FAIL\n");
    return 1;
}