#include <cstdio>
bool __fastcall CCreatureAction_ScorpionCharge_IsToFaceMovementDirection()
{
    return false;
}
int main()
{
    if (CCreatureAction_ScorpionCharge_IsToFaceMovementDirection() == false) { std::printf("AUTO_TINY_008c5bd0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c5bd0_TEST FAIL\n");
    return 1;
}