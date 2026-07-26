#include <cstdio>
bool __fastcall CCreatureAction_BeingForcePushed_IsUsingAnimationMovement()
{
    return false;
}
int main()
{
    if (CCreatureAction_BeingForcePushed_IsUsingAnimationMovement() == false) { std::printf("AUTO_TINY_008bdb30_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bdb30_TEST FAIL\n");
    return 1;
}