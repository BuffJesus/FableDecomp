#include <cstdio>
int __fastcall CGoldDef_GetSizeofClass()
{
    return 44;
}
int main()
{
    if (CGoldDef_GetSizeofClass() == 44) { std::printf("AUTO_TINY_004d584f_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d584f_TEST FAIL\n");
    return 1;
}