#include <cstdio>
int __fastcall CCrateStackDef_GetSizeofClass()
{
    return 48;
}
int main()
{
    if (CCrateStackDef_GetSizeofClass() == 48) { std::printf("AUTO_TINY_004d50b2_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d50b2_TEST FAIL\n");
    return 1;
}