#include <cstdio>
int __fastcall CShoveHaPennyRegionDef_GetSizeofClass()
{
    return 48;
}
int main()
{
    if (CShoveHaPennyRegionDef_GetSizeofClass() == 48) { std::printf("AUTO_TINY_004eafa3_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004eafa3_TEST FAIL\n");
    return 1;
}