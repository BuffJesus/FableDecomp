#include <cstdio>
int __fastcall CCreatureAction_StartBlocking_GetAnimationTransitionInTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_StartBlocking_GetAnimationTransitionInTime() == 0) { std::printf("AUTO_TINY_00855d60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00855d60_TEST FAIL\n");
    return 1;
}