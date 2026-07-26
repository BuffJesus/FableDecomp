#include <cstdio>
int __fastcall CCreatureAction_RiseUp_GetAnimationTransitionInTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_RiseUp_GetAnimationTransitionInTime() == 0) { std::printf("AUTO_TINY_0074db80_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0074db80_TEST FAIL\n");
    return 1;
}