#include <cstdio>
int __fastcall CBoastingPodiumDef_GetSizeofClass()
{
    return 44;
}
int main()
{
    if (CBoastingPodiumDef_GetSizeofClass() == 44) { std::printf("AUTO_TINY_004d46f5_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d46f5_TEST FAIL\n");
    return 1;
}