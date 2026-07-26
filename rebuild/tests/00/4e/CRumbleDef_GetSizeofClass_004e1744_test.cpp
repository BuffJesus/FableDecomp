#include <cstdio>
int __fastcall CRumbleDef_GetSizeofClass()
{
    return 64;
}
int main()
{
    if (CRumbleDef_GetSizeofClass() == 64) { std::printf("AUTO_TINY_004e1744_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e1744_TEST FAIL\n");
    return 1;
}