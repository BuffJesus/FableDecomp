#include <cstdio>
int __fastcall CCreatureAction_BanditKingUnstuck_GetAnimationTransitionInTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_BanditKingUnstuck_GetAnimationTransitionInTime() == 0) { std::printf("AUTO_TINY_008c9600_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c9600_TEST FAIL\n");
    return 1;
}