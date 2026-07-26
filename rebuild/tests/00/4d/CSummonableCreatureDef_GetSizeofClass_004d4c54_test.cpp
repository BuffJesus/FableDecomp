#include <cstdio>
int __fastcall CSummonableCreatureDef_GetSizeofClass()
{
    return 48;
}
int main()
{
    if (CSummonableCreatureDef_GetSizeofClass() == 48) { std::printf("AUTO_TINY_004d4c54_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d4c54_TEST FAIL\n");
    return 1;
}