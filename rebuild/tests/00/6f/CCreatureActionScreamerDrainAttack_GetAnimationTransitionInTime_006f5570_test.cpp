#include <cstdio>
int __fastcall CCreatureAction_ScreamerDrainAttack_GetAnimationTransitionInTime()
{
    return 15;
}
int main()
{
    if (CCreatureAction_ScreamerDrainAttack_GetAnimationTransitionInTime() == 15) { std::printf("AUTO_TINY_006f5570_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006f5570_TEST FAIL\n");
    return 1;
}