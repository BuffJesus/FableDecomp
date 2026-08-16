#include <cstdio>
int __fastcall C3DAnimationState_GetType()
{
    return 0;
}
int main()
{
    if (C3DAnimationState_GetType() == 0) { std::printf("AUTO_TINY_00550880_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00550880_TEST FAIL\n");
    return 1;
}