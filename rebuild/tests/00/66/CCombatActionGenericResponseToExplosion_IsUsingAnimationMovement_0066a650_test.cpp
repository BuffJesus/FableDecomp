#include <cstdio>
bool __fastcall CCombatAction_GenericResponseToExplosion_IsUsingAnimationMovement()
{
    return true;
}
int main()
{
    if (CCombatAction_GenericResponseToExplosion_IsUsingAnimationMovement() == true) { std::printf("AUTO_TINY_0066a650_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0066a650_TEST FAIL\n");
    return 1;
}