#include <cstdio>
int __fastcall CHeroSuitDef_GetSizeofClass()
{
    return 52;
}
int main()
{
    if (CHeroSuitDef_GetSizeofClass() == 52) { std::printf("AUTO_TINY_004e091a_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e091a_TEST FAIL\n");
    return 1;
}