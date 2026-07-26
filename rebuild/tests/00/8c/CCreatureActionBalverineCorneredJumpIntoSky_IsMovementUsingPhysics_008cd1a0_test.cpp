#include <cstdio>
bool __fastcall CCreatureAction_BalverineCorneredJumpIntoSky_IsMovementUsingPhysics()
{
    return false;
}
int main()
{
    if (CCreatureAction_BalverineCorneredJumpIntoSky_IsMovementUsingPhysics() == false) { std::printf("AUTO_TINY_008cd1a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cd1a0_TEST FAIL\n");
    return 1;
}