#include <cstdio>
int __fastcall CLightDef_GetSizeofClass()
{
    return 92;
}
int main()
{
    if (CLightDef_GetSizeofClass() == 92) { std::printf("AUTO_TINY_004d33d8_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d33d8_TEST FAIL\n");
    return 1;
}