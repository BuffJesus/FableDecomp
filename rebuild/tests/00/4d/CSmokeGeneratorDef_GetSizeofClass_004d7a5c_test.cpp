#include <cstdio>
int __fastcall CSmokeGeneratorDef_GetSizeofClass()
{
    return 48;
}
int main()
{
    if (CSmokeGeneratorDef_GetSizeofClass() == 48) { std::printf("AUTO_TINY_004d7a5c_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d7a5c_TEST FAIL\n");
    return 1;
}