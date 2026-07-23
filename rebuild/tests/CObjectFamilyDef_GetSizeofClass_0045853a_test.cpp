#include <cstdio>
int __fastcall CObjectFamilyDef_GetSizeofClass()
{
    return 72;
}
int main()
{
    if (CObjectFamilyDef_GetSizeofClass() == 72) { std::printf("AUTO_TINY_0045853a_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0045853a_TEST FAIL\n");
    return 1;
}