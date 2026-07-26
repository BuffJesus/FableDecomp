#include <cstdio>
int __fastcall CFishDef_GetSizeofClass()
{
    return 88;
}
int main()
{
    if (CFishDef_GetSizeofClass() == 88) { std::printf("AUTO_TINY_004d5c89_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d5c89_TEST FAIL\n");
    return 1;
}