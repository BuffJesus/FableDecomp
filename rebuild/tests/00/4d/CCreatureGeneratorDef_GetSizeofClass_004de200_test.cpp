#include <cstdio>
int __fastcall CCreatureGeneratorDef_GetSizeofClass()
{
    return 64;
}
int main()
{
    if (CCreatureGeneratorDef_GetSizeofClass() == 64) { std::printf("AUTO_TINY_004de200_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004de200_TEST FAIL\n");
    return 1;
}