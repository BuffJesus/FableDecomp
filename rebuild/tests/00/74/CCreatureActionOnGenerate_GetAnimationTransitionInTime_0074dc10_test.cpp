#include <cstdio>
int __fastcall CCreatureAction_OnGenerate_GetAnimationTransitionInTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_OnGenerate_GetAnimationTransitionInTime() == 0) { std::printf("AUTO_TINY_0074dc10_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0074dc10_TEST FAIL\n");
    return 1;
}