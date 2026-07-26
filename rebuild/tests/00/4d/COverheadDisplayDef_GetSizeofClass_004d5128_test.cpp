#include <cstdio>
int __fastcall COverheadDisplayDef_GetSizeofClass()
{
    return 40;
}
int main()
{
    if (COverheadDisplayDef_GetSizeofClass() == 40) { std::printf("AUTO_TINY_004d5128_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d5128_TEST FAIL\n");
    return 1;
}