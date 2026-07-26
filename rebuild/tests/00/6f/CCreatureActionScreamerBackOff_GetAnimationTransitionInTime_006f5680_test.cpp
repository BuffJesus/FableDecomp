#include <cstdio>
int __fastcall CCreatureAction_ScreamerBackOff_GetAnimationTransitionInTime()
{
    return 20;
}
int main()
{
    if (CCreatureAction_ScreamerBackOff_GetAnimationTransitionInTime() == 20) { std::printf("AUTO_TINY_006f5680_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006f5680_TEST FAIL\n");
    return 1;
}