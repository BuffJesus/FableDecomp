#include <cstdio>
int __fastcall CWifeDef_GetSizeofClass()
{
    return 44;
}
int main()
{
    if (CWifeDef_GetSizeofClass() == 44) { std::printf("AUTO_TINY_004d32a3_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d32a3_TEST FAIL\n");
    return 1;
}