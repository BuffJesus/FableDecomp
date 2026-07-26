#include <cstdio>
int __fastcall CCreatureAction_BanditKingStuck_GetAnimationTransitionInTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_BanditKingStuck_GetAnimationTransitionInTime() == 0) { std::printf("AUTO_TINY_008c9560_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c9560_TEST FAIL\n");
    return 1;
}