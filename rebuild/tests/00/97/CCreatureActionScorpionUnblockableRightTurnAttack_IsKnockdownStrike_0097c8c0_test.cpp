#include <cstdio>
bool __fastcall CCreatureAction_ScorpionUnblockableRightTurnAttack_IsKnockdownStrike()
{
    return true;
}
int main()
{
    if (CCreatureAction_ScorpionUnblockableRightTurnAttack_IsKnockdownStrike() == true) { std::printf("AUTO_TINY_0097c8c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0097c8c0_TEST FAIL\n");
    return 1;
}