#include <cstdio>
int __fastcall CCreatureAction_BeStunned_GetAnimationTransitionInTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_BeStunned_GetAnimationTransitionInTime() == 0) { std::printf("AUTO_TINY_008659d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008659d0_TEST FAIL\n");
    return 1;
}