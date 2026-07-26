#include <cstdio>
int __fastcall CDecapitationDef_GetSizeofClass()
{
    return 48;
}
int main()
{
    if (CDecapitationDef_GetSizeofClass() == 48) { std::printf("AUTO_TINY_004d5b15_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d5b15_TEST FAIL\n");
    return 1;
}