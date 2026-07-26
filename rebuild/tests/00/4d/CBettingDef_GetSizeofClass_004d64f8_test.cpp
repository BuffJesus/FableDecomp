#include <cstdio>
int __fastcall CBettingDef_GetSizeofClass()
{
    return 88;
}
int main()
{
    if (CBettingDef_GetSizeofClass() == 88) { std::printf("AUTO_TINY_004d64f8_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d64f8_TEST FAIL\n");
    return 1;
}