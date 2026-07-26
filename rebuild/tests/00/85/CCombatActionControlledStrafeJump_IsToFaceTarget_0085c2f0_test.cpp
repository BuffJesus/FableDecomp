#include <cstdio>
bool __fastcall CCombatAction_ControlledStrafeJump_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCombatAction_ControlledStrafeJump_IsToFaceTarget() == true) { std::printf("AUTO_TINY_0085c2f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0085c2f0_TEST FAIL\n");
    return 1;
}