#include <cstdio>
bool __fastcall CCreatureAction_BalverineCorneredJumpIntoSky_IsToCauseRecoil()
{
    return true;
}
int main()
{
    if (CCreatureAction_BalverineCorneredJumpIntoSky_IsToCauseRecoil() == true) { std::printf("AUTO_TINY_008cd1b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cd1b0_TEST FAIL\n");
    return 1;
}