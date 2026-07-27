#include <cstdio>
bool __fastcall CCreatureAction_ScorpionChargeInto_IsToFaceMovementDirection()
{
    return false;
}
int main()
{
    if (CCreatureAction_ScorpionChargeInto_IsToFaceMovementDirection() == false) { std::printf("AUTO_TINY_008c5ba0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c5ba0_TEST FAIL\n");
    return 1;
}