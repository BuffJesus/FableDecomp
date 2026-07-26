#include <cstdio>
int __fastcall CCreatureAction_Die_GetAnimationTransitionInTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_Die_GetAnimationTransitionInTime() == 0) { std::printf("AUTO_TINY_008edb50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008edb50_TEST FAIL\n");
    return 1;
}