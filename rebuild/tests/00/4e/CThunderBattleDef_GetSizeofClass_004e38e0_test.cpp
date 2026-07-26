#include <cstdio>
int __fastcall CThunderBattleDef_GetSizeofClass()
{
    return 76;
}
int main()
{
    if (CThunderBattleDef_GetSizeofClass() == 76) { std::printf("AUTO_TINY_004e38e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e38e0_TEST FAIL\n");
    return 1;
}