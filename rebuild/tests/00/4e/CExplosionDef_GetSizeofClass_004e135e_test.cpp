#include <cstdio>
int __fastcall CExplosionDef_GetSizeofClass()
{
    return 112;
}
int main()
{
    if (CExplosionDef_GetSizeofClass() == 112) { std::printf("AUTO_TINY_004e135e_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e135e_TEST FAIL\n");
    return 1;
}