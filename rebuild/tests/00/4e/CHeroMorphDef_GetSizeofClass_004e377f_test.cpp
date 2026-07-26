#include <cstdio>
int __fastcall CHeroMorphDef_GetSizeofClass()
{
    return 104;
}
int main()
{
    if (CHeroMorphDef_GetSizeofClass() == 104) { std::printf("AUTO_TINY_004e377f_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e377f_TEST FAIL\n");
    return 1;
}