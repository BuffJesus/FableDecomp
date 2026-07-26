#include <cstdio>
int __fastcall CAppearanceDef_GetSizeofClass()
{
    return 120;
}
int main()
{
    if (CAppearanceDef_GetSizeofClass() == 120) { std::printf("AUTO_TINY_0046a174_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0046a174_TEST FAIL\n");
    return 1;
}