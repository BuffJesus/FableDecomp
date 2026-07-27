#include <cstdio>
bool __fastcall CCreatureAction_ScorpionChargeStrike_IsToFaceMovementDirection()
{
    return false;
}
int main()
{
    if (CCreatureAction_ScorpionChargeStrike_IsToFaceMovementDirection() == false) { std::printf("AUTO_TINY_008c5c20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c5c20_TEST FAIL\n");
    return 1;
}