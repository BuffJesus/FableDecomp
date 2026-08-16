#include <cstdio>
int __fastcall C3DAnimationStateAlpha_GetType()
{
    return 1;
}
int main()
{
    if (C3DAnimationStateAlpha_GetType() == 1) { std::printf("AUTO_TINY_0070fe20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0070fe20_TEST FAIL\n");
    return 1;
}