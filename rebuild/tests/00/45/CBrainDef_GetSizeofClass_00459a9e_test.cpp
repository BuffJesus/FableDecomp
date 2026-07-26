#include <cstdio>
int __fastcall CBrainDef_GetSizeofClass()
{
    return 96;
}
int main()
{
    if (CBrainDef_GetSizeofClass() == 96) { std::printf("AUTO_TINY_00459a9e_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00459a9e_TEST FAIL\n");
    return 1;
}