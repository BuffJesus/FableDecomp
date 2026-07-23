#include <cstdio>
int __fastcall CSoundDef_GetSizeofClass()
{
    return 132;
}
int main()
{
    if (CSoundDef_GetSizeofClass() == 132) { std::printf("AUTO_TINY_00458454_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00458454_TEST FAIL\n");
    return 1;
}