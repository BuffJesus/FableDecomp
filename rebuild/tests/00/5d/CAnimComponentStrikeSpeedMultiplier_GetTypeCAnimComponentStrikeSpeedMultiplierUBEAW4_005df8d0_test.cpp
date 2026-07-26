#include <cstdio>
int __fastcall CAnimComponentStrikeSpeedMultiplier_GetType()
{
    return 9;
}
int main()
{
    if (CAnimComponentStrikeSpeedMultiplier_GetType() == 9) { std::printf("AUTO_TINY_005df8d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_005df8d0_TEST FAIL\n");
    return 1;
}