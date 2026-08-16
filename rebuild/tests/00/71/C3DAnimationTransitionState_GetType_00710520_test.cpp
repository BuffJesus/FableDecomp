#include <cstdio>
int __fastcall C3DAnimationTransitionState_GetType()
{
    return 2;
}
int main()
{
    if (C3DAnimationTransitionState_GetType() == 2) { std::printf("AUTO_TINY_00710520_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00710520_TEST FAIL\n");
    return 1;
}