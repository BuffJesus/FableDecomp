#include <cstdio>
int __fastcall CCreatureModeDef_GetSizeofClass()
{
    return 64;
}
int main()
{
    if (CCreatureModeDef_GetSizeofClass() == 64) { std::printf("AUTO_TINY_004de7f9_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004de7f9_TEST FAIL\n");
    return 1;
}