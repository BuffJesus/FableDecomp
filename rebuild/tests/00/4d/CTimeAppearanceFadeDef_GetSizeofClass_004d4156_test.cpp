#include <cstdio>
int __fastcall CTimeAppearanceFadeDef_GetSizeofClass()
{
    return 56;
}
int main()
{
    if (CTimeAppearanceFadeDef_GetSizeofClass() == 56) { std::printf("AUTO_TINY_004d4156_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d4156_TEST FAIL\n");
    return 1;
}