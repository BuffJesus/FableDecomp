#include <cstdio>
int __fastcall CSBCCorridor_GetType()
{
    return 3;
}
int main()
{
    if (CSBCCorridor_GetType() == 3) { std::printf("AUTO_TINY_00a21f90_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a21f90_TEST FAIL\n");
    return 1;
}