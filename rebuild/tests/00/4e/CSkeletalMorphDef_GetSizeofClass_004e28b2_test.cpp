#include <cstdio>
int __fastcall CSkeletalMorphDef_GetSizeofClass()
{
    return 52;
}
int main()
{
    if (CSkeletalMorphDef_GetSizeofClass() == 52) { std::printf("AUTO_TINY_004e28b2_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e28b2_TEST FAIL\n");
    return 1;
}