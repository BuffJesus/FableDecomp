#include <cstdio>
int __fastcall CGiftDef_GetSizeofClass()
{
    return 48;
}
int main()
{
    if (CGiftDef_GetSizeofClass() == 48) { std::printf("AUTO_TINY_004d41ab_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d41ab_TEST FAIL\n");
    return 1;
}