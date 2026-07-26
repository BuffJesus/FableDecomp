#include <cstdio>
int __fastcall CWallMountEffectsDef_GetSizeofClass()
{
    return 52;
}
int main()
{
    if (CWallMountEffectsDef_GetSizeofClass() == 52) { std::printf("AUTO_TINY_004d5c43_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d5c43_TEST FAIL\n");
    return 1;
}