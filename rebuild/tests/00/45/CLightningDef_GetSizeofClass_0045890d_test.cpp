#include <cstdio>
int __fastcall CLightningDef_GetSizeofClass()
{
    return 88;
}
int main()
{
    if (CLightningDef_GetSizeofClass() == 88) { std::printf("AUTO_TINY_0045890d_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0045890d_TEST FAIL\n");
    return 1;
}