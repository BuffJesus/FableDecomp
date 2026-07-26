#include <cstdio>
int __fastcall CJackOfBladesBattleDef_GetSizeofClass()
{
    return 128;
}
int main()
{
    if (CJackOfBladesBattleDef_GetSizeofClass() == 128) { std::printf("AUTO_TINY_004e39e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e39e0_TEST FAIL\n");
    return 1;
}