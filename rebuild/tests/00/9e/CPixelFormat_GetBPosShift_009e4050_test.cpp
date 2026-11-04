#include <cstdio>
int __fastcall CPixelFormat_GetBPosShift()
{
    return 0;
}
int main()
{
    if (CPixelFormat_GetBPosShift() == 0) { std::printf("AUTO_TINY_009e4050_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009e4050_TEST FAIL\n");
    return 1;
}