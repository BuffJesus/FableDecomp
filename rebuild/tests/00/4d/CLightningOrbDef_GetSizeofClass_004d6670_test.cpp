#include <cstdio>
int __fastcall CLightningOrbDef_GetSizeofClass()
{
    return 60;
}
int main()
{
    if (CLightningOrbDef_GetSizeofClass() == 60) { std::printf("AUTO_TINY_004d6670_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d6670_TEST FAIL\n");
    return 1;
}