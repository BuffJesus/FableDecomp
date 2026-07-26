#include <cstdio>
int __fastcall CLookDef_GetSizeofClass()
{
    return 88;
}
int main()
{
    if (CLookDef_GetSizeofClass() == 88) { std::printf("AUTO_TINY_004d3938_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d3938_TEST FAIL\n");
    return 1;
}