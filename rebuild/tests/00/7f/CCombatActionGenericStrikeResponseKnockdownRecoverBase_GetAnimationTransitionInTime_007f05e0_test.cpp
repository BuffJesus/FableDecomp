#include <cstdio>
int __fastcall CCombatAction_GenericStrikeResponseKnockdownRecoverBase_GetAnimationTransitionInTime()
{
    return 0;
}
int main()
{
    if (CCombatAction_GenericStrikeResponseKnockdownRecoverBase_GetAnimationTransitionInTime() == 0) { std::printf("AUTO_TINY_007f05e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_007f05e0_TEST FAIL\n");
    return 1;
}