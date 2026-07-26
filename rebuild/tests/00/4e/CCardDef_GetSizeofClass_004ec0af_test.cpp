#include <cstdio>
int __fastcall CCardDef_GetSizeofClass()
{
    return 68;
}
int main()
{
    if (CCardDef_GetSizeofClass() == 68) { std::printf("AUTO_TINY_004ec0af_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004ec0af_TEST FAIL\n");
    return 1;
}