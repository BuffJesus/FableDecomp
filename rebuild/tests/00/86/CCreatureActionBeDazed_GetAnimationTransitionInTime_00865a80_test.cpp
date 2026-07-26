#include <cstdio>
int __fastcall CCreatureAction_BeDazed_GetAnimationTransitionInTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_BeDazed_GetAnimationTransitionInTime() == 0) { std::printf("AUTO_TINY_00865a80_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00865a80_TEST FAIL\n");
    return 1;
}