#include <cstdio>
bool __fastcall CCreatureAction_ScorpionUnblockableLeftTurnAttack_IsToFaceMovementDirection()
{
    return false;
}
int main()
{
    if (CCreatureAction_ScorpionUnblockableLeftTurnAttack_IsToFaceMovementDirection() == false) { std::printf("AUTO_TINY_0097c800_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0097c800_TEST FAIL\n");
    return 1;
}