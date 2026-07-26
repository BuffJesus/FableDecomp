#include <cstdio>
int __fastcall CCreatureAction_UndeadReturnToEarth_GetAnimationTransitionInTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_UndeadReturnToEarth_GetAnimationTransitionInTime() == 0) { std::printf("AUTO_TINY_008ec780_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008ec780_TEST FAIL\n");
    return 1;
}