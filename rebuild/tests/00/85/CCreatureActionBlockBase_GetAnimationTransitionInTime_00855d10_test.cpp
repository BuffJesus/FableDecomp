#include <cstdio>
int __fastcall CCreatureAction_BlockBase_GetAnimationTransitionInTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_BlockBase_GetAnimationTransitionInTime() == 0) { std::printf("AUTO_TINY_00855d10_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00855d10_TEST FAIL\n");
    return 1;
}