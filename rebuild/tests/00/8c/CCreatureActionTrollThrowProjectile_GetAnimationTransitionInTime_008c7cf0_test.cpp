#include <cstdio>
int __fastcall CCreatureAction_TrollThrowProjectile_GetAnimationTransitionInTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_TrollThrowProjectile_GetAnimationTransitionInTime() == 0) { std::printf("AUTO_TINY_008c7cf0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c7cf0_TEST FAIL\n");
    return 1;
}