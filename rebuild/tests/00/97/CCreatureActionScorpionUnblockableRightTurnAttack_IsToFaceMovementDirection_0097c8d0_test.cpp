#include <cstdio>
bool __fastcall CCreatureAction_ScorpionUnblockableRightTurnAttack_IsToFaceMovementDirection()
{
    return false;
}
int main()
{
    if (CCreatureAction_ScorpionUnblockableRightTurnAttack_IsToFaceMovementDirection() == false) { std::printf("AUTO_TINY_0097c8d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0097c8d0_TEST FAIL\n");
    return 1;
}