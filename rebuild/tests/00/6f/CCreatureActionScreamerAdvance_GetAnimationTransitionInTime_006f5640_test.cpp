#include <cstdio>
int __fastcall CCreatureAction_ScreamerAdvance_GetAnimationTransitionInTime()
{
    return 30;
}
int main()
{
    if (CCreatureAction_ScreamerAdvance_GetAnimationTransitionInTime() == 30) { std::printf("AUTO_TINY_006f5640_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006f5640_TEST FAIL\n");
    return 1;
}