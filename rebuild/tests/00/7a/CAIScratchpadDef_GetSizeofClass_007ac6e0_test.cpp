#include <cstdio>
int __fastcall CAIScratchpadDef_GetSizeofClass()
{
    return 156;
}
int main()
{
    if (CAIScratchpadDef_GetSizeofClass() == 156) { std::printf("AUTO_TINY_007ac6e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_007ac6e0_TEST FAIL\n");
    return 1;
}