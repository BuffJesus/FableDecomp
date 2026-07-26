#include <cstdio>
int __fastcall CTurncoatDef_GetSizeofClass()
{
    return 84;
}
int main()
{
    if (CTurncoatDef_GetSizeofClass() == 84) { std::printf("AUTO_TINY_004debc0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004debc0_TEST FAIL\n");
    return 1;
}