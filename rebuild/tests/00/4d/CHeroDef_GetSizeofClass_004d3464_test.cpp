#include <cstdio>
int __fastcall CHeroDef_GetSizeofClass()
{
    return 48;
}
int main()
{
    if (CHeroDef_GetSizeofClass() == 48) { std::printf("AUTO_TINY_004d3464_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d3464_TEST FAIL\n");
    return 1;
}