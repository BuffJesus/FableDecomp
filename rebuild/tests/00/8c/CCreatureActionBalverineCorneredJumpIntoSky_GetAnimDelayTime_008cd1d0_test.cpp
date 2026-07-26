#include <cstdio>
int __fastcall CCreatureAction_BalverineCorneredJumpIntoSky_GetAnimDelayTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_BalverineCorneredJumpIntoSky_GetAnimDelayTime() == 0) { std::printf("AUTO_TINY_008cd1d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cd1d0_TEST FAIL\n");
    return 1;
}