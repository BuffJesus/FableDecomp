#include <cstdio>
int __fastcall CFishingDef_GetSizeofClass()
{
    return 124;
}
int main()
{
    if (CFishingDef_GetSizeofClass() == 124) { std::printf("AUTO_TINY_004de912_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004de912_TEST FAIL\n");
    return 1;
}