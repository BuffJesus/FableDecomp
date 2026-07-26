#include <cstdio>
int __fastcall CCreatureAction_ScreamerIdle_GetAnimationTransitionInTime()
{
    return 15;
}
int main()
{
    if (CCreatureAction_ScreamerIdle_GetAnimationTransitionInTime() == 15) { std::printf("AUTO_TINY_006f5600_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006f5600_TEST FAIL\n");
    return 1;
}