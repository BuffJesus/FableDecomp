#include <cstdio>
int __fastcall CCreatureAction_ScreamerDrainOutOf_GetAnimationTransitionInTime()
{
    return 10;
}
int main()
{
    if (CCreatureAction_ScreamerDrainOutOf_GetAnimationTransitionInTime() == 10) { std::printf("AUTO_TINY_006f55c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006f55c0_TEST FAIL\n");
    return 1;
}