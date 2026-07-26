#include <cstdio>
int __fastcall CExpressionSubDef_GetSizeofClass()
{
    return 44;
}
int main()
{
    if (CExpressionSubDef_GetSizeofClass() == 44) { std::printf("AUTO_TINY_004d4b97_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d4b97_TEST FAIL\n");
    return 1;
}