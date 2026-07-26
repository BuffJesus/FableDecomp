#include <cstdio>
int __fastcall CFlammableDef_GetSizeofClass()
{
    return 76;
}
int main()
{
    if (CFlammableDef_GetSizeofClass() == 76) { std::printf("AUTO_TINY_004e2865_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e2865_TEST FAIL\n");
    return 1;
}