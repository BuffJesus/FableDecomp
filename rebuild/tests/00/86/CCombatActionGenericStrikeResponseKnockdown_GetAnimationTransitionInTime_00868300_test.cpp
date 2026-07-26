#include <cstdio>
int __fastcall CCombatAction_GenericStrikeResponseKnockdown_GetAnimationTransitionInTime()
{
    return 0;
}
int main()
{
    if (CCombatAction_GenericStrikeResponseKnockdown_GetAnimationTransitionInTime() == 0) { std::printf("AUTO_TINY_00868300_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00868300_TEST FAIL\n");
    return 1;
}