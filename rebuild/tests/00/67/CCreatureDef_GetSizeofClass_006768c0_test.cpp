#include <cstdio>
int __fastcall CCreatureDef_GetSizeofClass()
{
    return 232;
}
int main()
{
    if (CCreatureDef_GetSizeofClass() == 232) { std::printf("AUTO_TINY_006768c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006768c0_TEST FAIL\n");
    return 1;
}