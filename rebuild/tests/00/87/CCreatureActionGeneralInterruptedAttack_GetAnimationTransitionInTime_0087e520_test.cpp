#include <cstdio>
int __fastcall CCreatureAction_GeneralInterruptedAttack_GetAnimationTransitionInTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_GeneralInterruptedAttack_GetAnimationTransitionInTime() == 0) { std::printf("AUTO_TINY_0087e520_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0087e520_TEST FAIL\n");
    return 1;
}