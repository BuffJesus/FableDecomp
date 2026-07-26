#include <cstdio>
int __fastcall CCreatureAction_BanditKingLunge_GetAnimationTransitionInTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_BanditKingLunge_GetAnimationTransitionInTime() == 0) { std::printf("AUTO_TINY_00961a00_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00961a00_TEST FAIL\n");
    return 1;
}