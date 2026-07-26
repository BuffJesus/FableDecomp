#include <cstdio>
int __fastcall CGuardDef_GetSizeofClass()
{
    return 80;
}
int main()
{
    if (CGuardDef_GetSizeofClass() == 80) { std::printf("AUTO_TINY_004d500f_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d500f_TEST FAIL\n");
    return 1;
}