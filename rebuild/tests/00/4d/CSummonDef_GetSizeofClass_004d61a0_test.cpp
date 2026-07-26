#include <cstdio>
int __fastcall CSummonDef_GetSizeofClass()
{
    return 76;
}
int main()
{
    if (CSummonDef_GetSizeofClass() == 76) { std::printf("AUTO_TINY_004d61a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d61a0_TEST FAIL\n");
    return 1;
}