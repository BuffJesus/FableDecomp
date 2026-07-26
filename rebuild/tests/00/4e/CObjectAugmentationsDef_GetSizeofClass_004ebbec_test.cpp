#include <cstdio>
int __fastcall CObjectAugmentationsDef_GetSizeofClass()
{
    return 140;
}
int main()
{
    if (CObjectAugmentationsDef_GetSizeofClass() == 140) { std::printf("AUTO_TINY_004ebbec_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004ebbec_TEST FAIL\n");
    return 1;
}