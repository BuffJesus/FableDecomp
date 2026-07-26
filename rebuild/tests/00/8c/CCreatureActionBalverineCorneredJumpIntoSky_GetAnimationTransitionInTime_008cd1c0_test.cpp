#include <cstdio>
int __fastcall CCreatureAction_BalverineCorneredJumpIntoSky_GetAnimationTransitionInTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_BalverineCorneredJumpIntoSky_GetAnimationTransitionInTime() == 0) { std::printf("AUTO_TINY_008cd1c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cd1c0_TEST FAIL\n");
    return 1;
}